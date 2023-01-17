#include <simple3d/context/window.h>

#include <memory>
#include <unordered_map>
#include <utility>

#include <glad/gles2.h>
#include <GLFW/glfw3.h>

#include <simple3d/context/input.h>
#include <simple3d/misc/error.h>

namespace Simple3D {



class FramebufferSizeHandler : public IWindowInputHandler {
 public:
  FramebufferSizeHandler() = default;
  ~FramebufferSizeHandler() override = default;

  void FramebufferSizeCallback(int width, int height) override {
    glViewport(0, 0, width, height);
  }
};
// static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//   glViewport(0, 0, width, height);
// }

Window Window::Create(Error* error) {
  glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  // doesn't work with ANGLE for some reason
  // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE); // disable vsync

  GLFWwindow* glfw_window = glfwCreateWindow(
    800, 800, "test (FIXME)", nullptr, nullptr);

  if (glfw_window == nullptr) {
    *error = Error(ErrorType::kWindowCreationFailed,
        "glfw failed to create window");
    return Window{};
  }

  glfwMakeContextCurrent(glfw_window);

  // discard the version, because it must be compatible with
  // OpenGL ES 3.1, and the library isn't using anything
  // that isn't available in GLES 3.1
  gladLoadGLES2(glfwGetProcAddress);

  *error = Error::Ok();
  return Window(glfw_window);
}

Window::Window(GLFWwindow* window): window_{window} {
  EnableWindowInputHandler(
    std::dynamic_pointer_cast<IWindowInputHandler>(std::make_shared<FramebufferSizeHandler>()));
}

Window::Window(Window&& window)
    : window_{window.window_}, 
      input_handlers_{std::move(window.input_handlers_)}, 
      window_input_handlers_{std::move(window.window_input_handlers_)} {
  window_ = nullptr;
}

Window& Window::operator=(Window&& window) {
  if (&window == this)
    return *this;
  
  if (window_ != nullptr) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }

  std::swap(window_, window.window_);
  input_handlers_ = std::move(window.input_handlers_);
  window_input_handlers_ = std::move(window.window_input_handlers_);

  return *this;
}

Window::~Window() {
  if (window_ != nullptr) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }
}

void Window::SwapBuffers() {
  glfwSwapBuffers(window_);
}

bool Window::ShouldClose() {
  return glfwWindowShouldClose(window_);
}

void Window::EnableInputHandler(const std::shared_ptr<IInputHandler>& input_handler) {
  input_handlers_.insert(input_handler);
}

void Window::EnableWindowInputHandler(
    const std::shared_ptr<IWindowInputHandler>& window_input_handler) {
  window_input_handlers_.insert(window_input_handler);
}

void Window::DisableInputHandler(const std::shared_ptr<IInputHandler>& input_handler) {
  input_handlers_.erase(input_handler);
}

void Window::DisableWindowInputHandler(
    const std::shared_ptr<IWindowInputHandler>& window_input_handler) {
  window_input_handlers_.erase(window_input_handler);
}



void Window::KeyCallback(int key, int scancode, int action, int mods) {
  for (auto& input_handler : input_handlers_) {
    input_handler->KeyCallback(key, scancode, action, mods);
  }
}
void Window::CharCallback(unsigned int codepoint) {
  for (auto& input_handler : input_handlers_) {
    input_handler->CharCallback(codepoint);
  }
}
void Window::CharModsCallback(unsigned int codepoint, int mods) {
  for (auto& input_handler : input_handlers_) {
    input_handler->CharModsCallback(codepoint, mods);
  }
}
void Window::MouseButtonCallback(int button, int action, int mods) {
  for (auto& input_handler : input_handlers_) {
    input_handler->MouseButtonCallback(button, action, mods);
  }
}
void Window::CursorPosCallback(double xpos, double ypos) {
  for (auto& input_handler : input_handlers_) {
    input_handler->CursorPosCallback(xpos, ypos);
  }
}
void Window::CursorEnterCallback(int entered) {
  for (auto& input_handler : input_handlers_) {
    input_handler->CursorEnterCallback(entered);
  }
}
void Window::ScrollCallback(double xoffset, double yoffset) {
  for (auto& input_handler : input_handlers_) {
    input_handler->ScrollCallback(xoffset, yoffset);
  }
}
void Window::DropCallback(int path_count, const char* paths[]) {
  for (auto& input_handler : input_handlers_) {
    input_handler->DropCallback(path_count, paths);
  }
}



void Window::WindowPosCallback(int xpos, int ypos) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowPosCallback(xpos, ypos);
  }
}
void Window::WindowSizeCallback(int width, int height) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowSizeCallback(width, height);
  }
}
void Window::WindowCloseCallback() {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowCloseCallback();
  }
}
void Window::WindowRefreshCallback() {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowRefreshCallback();
  }
}
void Window::WindowFocusCallback(int focused) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowFocusCallback(focused);
  }
}
void Window::WindowIconifyCallback(int iconified) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowIconifyCallback(iconified);
  }
}
void Window::WindowMaximizeCallback(int maximized) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowMaximizeCallback(maximized);
  }
}
void Window::FramebufferSizeCallback(int width, int height) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->FramebufferSizeCallback(width, height);
  }
}
void Window::WindowContentScaleCallback(float xscale, float yscale) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowContentScaleCallback(xscale, yscale);
  }
}



}  // namespace Simple3D
