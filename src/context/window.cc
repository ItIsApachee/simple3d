#include <simple3d/context/window.h>

#include <memory>
#include <unordered_map>

#include <glad/gles2.h>
#include <GLFW/glfw3.h>

#include <simple3d/context/input.h>

namespace Simple3D {



// class WindowManager {
//  public:
//   static WindowManager& GetInstance() {
//     static WindowManager window_manager{};
//     return window_manager;
//   }
  
//   ~WindowManager() = default;

//   std::shared_ptr<Window> GetWindow(GLFWwindow* window) {
//     return windows_[window];
//   }

//  private:
//   WindowManager() = default;

//   std::unordered_map<GLFWwindow*, std::shared_ptr<Window>> windows_{};
// };

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

std::shared_ptr<Window> Window::Create(GLFWwindow* window) {
  return std::shared_ptr<Window>(
    new Window(window));
}

Window::Window(GLFWwindow* window): window_{window} {
  // TODO(apachee): consider moving window creation here
  // so that Window follows RAII: creating Window instance
  // create window resource
  // TODO(apachee): set callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Window::~Window() {
  if (window_ != nullptr)
    Destroy();
}

void Window::AddInputHandler(std::shared_ptr<IInputHandler> input_handler) {
  input_handlers_.push_back(input_handler);
}

void Window::AddWindowInputHandler(
    std::shared_ptr<IWindowInputHandler> window_input_handler) {
  window_input_handlers_.push_back(window_input_handler);
}

void Window::SwapBuffers() {
  glfwSwapBuffers(window_);
}

void Window::Destroy() {
  if (window_ != nullptr) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }
}



}  // namespace Simple3D
