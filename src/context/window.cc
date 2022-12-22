#include <simple3d/context/window.h>

#include <memory>
#include <unordered_map>
#include <utility>

#include <glad/gles2.h>
#include <GLFW/glfw3.h>

#include <simple3d/context/input.h>
#include <simple3d/misc/error.h>

namespace Simple3D {



static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

Window Window::Create(Error* error) {
  glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  // doesn't work with ANGLE for some reason
  // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE); // disable vsync

  GLFWwindow* glfw_window = glfwCreateWindow(
    500, 500, "test (FIXME)", nullptr, nullptr);

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
  // TODO(apachee): setup callbacks(?): inputs, etc.
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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

void Window::EnableInputHandler(std::shared_ptr<IInputHandler> input_handler) {
  input_handlers_.insert(input_handler);
}

void Window::EnableWindowInputHandler(
    std::shared_ptr<IWindowInputHandler> window_input_handler) {
  window_input_handlers_.insert(window_input_handler);
}

void Window::DisableInputHandler(std::shared_ptr<IInputHandler> input_handler) {
  input_handlers_.erase(input_handler);
}

void Window::DisableWindowInputHandler(
    std::shared_ptr<IWindowInputHandler> window_input_handler) {
  window_input_handlers_.erase(window_input_handler);
}

void Window::SwapBuffers() {
  glfwSwapBuffers(window_);
}

bool Window::ShouldClose() {
  return glfwWindowShouldClose(window_);
}



}  // namespace Simple3D
