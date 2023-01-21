#include <GLFW/glfw3.h>
#include <simple3d/context/input.h>

namespace Simple3D {

static IInputHandler* input_handler_ptr = nullptr;

static void keycallback(GLFWwindow* window, int key, int scancode, int action,
                        int mods) {
  if (input_handler_ptr == nullptr) return;
  input_handler_ptr->KeyCallback(key, scancode, action, mods);
}
static void charcallback(GLFWwindow* window, unsigned int codepoint) {
  if (input_handler_ptr == nullptr) return;
  input_handler_ptr->CharCallback(codepoint);
}
static void charmodscallback(GLFWwindow* window, unsigned int codepoint,
                             int mods) {
  if (input_handler_ptr == nullptr) return;
  input_handler_ptr->CharModsCallback(codepoint, mods);
}
static void mousebuttoncallback(GLFWwindow* window, int button, int action,
                                int mods) {
  if (input_handler_ptr == nullptr) return;
  input_handler_ptr->MouseButtonCallback(button, action, mods);
}
static void cursorposcallback(GLFWwindow* window, double xpos, double ypos) {
  if (input_handler_ptr == nullptr) return;
  input_handler_ptr->CursorPosCallback(xpos, ypos);
}
static void cursortenterCallback(GLFWwindow* window, int entered) {
  if (input_handler_ptr == nullptr) return;
  input_handler_ptr->CursorEnterCallback(entered);
}
static void scrollcallback(GLFWwindow* window, double xoffset, double yoffset) {
  if (input_handler_ptr == nullptr) return;
  input_handler_ptr->ScrollCallback(xoffset, yoffset);
}
static void dropcallback(GLFWwindow* window, int path_count,
                         const char* paths[]) {
  if (input_handler_ptr == nullptr) return;
  input_handler_ptr->DropCallback(path_count, paths);
}

static IWindowInputHandler* window_input_handler_ptr = nullptr;

void windowposcallback(GLFWwindow* window, int xpos, int ypos) {
  if (window_input_handler_ptr == nullptr) return;
  window_input_handler_ptr->WindowPosCallback(xpos, ypos);
}
void windowsizecallback(GLFWwindow* window, int width, int height) {
  if (window_input_handler_ptr == nullptr) return;
  window_input_handler_ptr->WindowSizeCallback(width, height);
}
void windowclosecallback(GLFWwindow* window) {
  if (window_input_handler_ptr == nullptr) return;
  window_input_handler_ptr->WindowCloseCallback();
}
void windowrefreshcallback(GLFWwindow* window) {
  if (window_input_handler_ptr == nullptr) return;
  window_input_handler_ptr->WindowRefreshCallback();
}
void windowfocuscallback(GLFWwindow* window, int focused) {
  if (window_input_handler_ptr == nullptr) return;
  window_input_handler_ptr->WindowFocusCallback(focused);
}
void windowiconifycallback(GLFWwindow* window, int iconified) {
  if (window_input_handler_ptr == nullptr) return;
  window_input_handler_ptr->WindowIconifyCallback(iconified);
}
void windowmaximizecallback(GLFWwindow* window, int maximized) {
  if (window_input_handler_ptr == nullptr) return;
  window_input_handler_ptr->WindowMaximizeCallback(maximized);
}
void framebuffersizecallback(GLFWwindow* window, int width, int height) {
  if (window_input_handler_ptr == nullptr) return;
  window_input_handler_ptr->FramebufferSizeCallback(width, height);
}
void windowcontentscalecallback(GLFWwindow* window, float xscale,
                                float yscale) {
  if (window_input_handler_ptr == nullptr) return;
  window_input_handler_ptr->WindowContentScaleCallback(xscale, yscale);
}

namespace Internal {

void SetInputHandler(GLFWwindow* window, IInputHandler* input_handler_ptr_) {
  input_handler_ptr = input_handler_ptr_;

  glfwSetKeyCallback(window, keycallback);
  glfwSetCharCallback(window, charcallback);
  glfwSetCharModsCallback(window, charmodscallback);
  glfwSetMouseButtonCallback(window, mousebuttoncallback);
  glfwSetCursorPosCallback(window, cursorposcallback);
  glfwSetCursorEnterCallback(window, cursortenterCallback);
  glfwSetScrollCallback(window, scrollcallback);
  glfwSetDropCallback(window, dropcallback);
}

void UnsetInputHandler() { input_handler_ptr = nullptr; }

void SetWindowInputHandler(GLFWwindow* window,
                           IWindowInputHandler* win_handler_ptr_) {
  window_input_handler_ptr = win_handler_ptr_;

  glfwSetWindowPosCallback(window, windowposcallback);
  glfwSetWindowSizeCallback(window, windowsizecallback);
  glfwSetWindowCloseCallback(window, windowclosecallback);
  glfwSetWindowRefreshCallback(window, windowrefreshcallback);
  glfwSetWindowFocusCallback(window, windowfocuscallback);
  glfwSetWindowIconifyCallback(window, windowiconifycallback);
  glfwSetWindowMaximizeCallback(window, windowmaximizecallback);
  glfwSetFramebufferSizeCallback(window, framebuffersizecallback);
  glfwSetWindowContentScaleCallback(window, windowcontentscalecallback);
}

void UnsetWindowInputHandler() { window_input_handler_ptr = nullptr; }

}  // namespace Internal

void IInputHandler::KeyCallback(int key, int scancode, int action, int mods) {}
void IInputHandler::CharCallback(unsigned int codepoint) {}
void IInputHandler::CharModsCallback(unsigned int codepoint, int mods) {}
void IInputHandler::MouseButtonCallback(int button, int action, int mods) {}
void IInputHandler::CursorPosCallback(double xpos, double ypos) {}
void IInputHandler::CursorEnterCallback(int entered) {}
void IInputHandler::ScrollCallback(double xoffset, double yoffset) {}
void IInputHandler::DropCallback(int path_count, const char* paths[]) {}

void IWindowInputHandler::WindowPosCallback(int xpos, int ypos) {}
void IWindowInputHandler::WindowSizeCallback(int width, int height) {}
void IWindowInputHandler::WindowCloseCallback() {}
void IWindowInputHandler::WindowRefreshCallback() {}
void IWindowInputHandler::WindowFocusCallback(int focused) {}
void IWindowInputHandler::WindowIconifyCallback(int iconified) {}
void IWindowInputHandler::WindowMaximizeCallback(int maximized) {}
void IWindowInputHandler::FramebufferSizeCallback(int width, int height) {}
void IWindowInputHandler::WindowContentScaleCallback(float xscale,
                                                     float yscale) {}

}  // namespace Simple3D
