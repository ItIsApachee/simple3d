#include <simple3d/context/window.h>

#include <memory>

#include <glad/gles2.h>
#include <GLFW/glfw3.h>

namespace Simple3D {



static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

std::shared_ptr<Window> Window::Create(GLFWwindow* window) {
  return std::shared_ptr<Window>(
    new Window(window));
}

Window::Window(GLFWwindow* window): window_{window} {
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Window::~Window() {
  if (window_ != nullptr)
    Destroy();
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
