#ifndef INCLUDE_SIMPLE3D_CONTEXT_INPUT_H_
#define INCLUDE_SIMPLE3D_CONTEXT_INPUT_H_

#include <GLFW/glfw3.h>

namespace Simple3D {

class IInputHandler {
 public:
  IInputHandler() = default;
  virtual ~IInputHandler() = default;

  virtual void KeyCallback(int key, int scancode, int action, int mods);
  virtual void CharCallback(unsigned int codepoint);
  virtual void CharModsCallback(unsigned int codepoint, int mods);
  virtual void MouseButtonCallback(int button, int action, int mods);
  virtual void CursorPosCallback(double xpos, double ypos);
  virtual void CursorEnterCallback(int entered);
  virtual void ScrollCallback(double xoffset, double yoffset);
  virtual void DropCallback(int path_count, const char* paths[]);
};

class IWindowInputHandler {
 public:
  IWindowInputHandler() = default;
  virtual ~IWindowInputHandler() = default;

  virtual void WindowPosCallback(int xpos, int ypos);
  virtual void WindowSizeCallback(int width, int height);
  virtual void WindowCloseCallback();
  virtual void WindowRefreshCallback();
  virtual void WindowFocusCallback(int focused);
  virtual void WindowIconifyCallback(int iconified);
  virtual void WindowMaximizeCallback(int maximized);
  virtual void FramebufferSizeCallback(int width, int height);
  virtual void WindowContentScaleCallback(float xscale, float yscale);
};

namespace Internal {

void SetInputHandler(GLFWwindow* window, IInputHandler*);
void UnsetInputHandler();

void SetWindowInputHandler(GLFWwindow* window, IWindowInputHandler*);
void UnsetWindowInputHandler();

}  // namespace Internal

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_CONTEXT_INPUT_H_
