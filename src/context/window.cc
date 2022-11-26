#include <simple3d/context/window.h>

#include <memory>

#include <GLFW/glfw3.h>

namespace Simple3D {



std::shared_ptr<Window> Window::Create(GLFWwindow* window) {
  return std::shared_ptr<Window>(
    new Window(window));
}

Window::Window(GLFWwindow* window): window_{window} {}

Window::~Window() {
  if (window_ != nullptr)
    Destroy();
}

void Window::Destroy() {
  if (window_ != nullptr) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }
}



}  // namespace Simple3D
