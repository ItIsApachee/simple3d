#ifndef INCLUDE_SIMPLE3D_CONTEXT_INPUT_H_
#define INCLUDE_SIMPLE3D_CONTEXT_INPUT_H_

namespace Simple3D {



class IInputHandler {
 public:
  IInputHandler() = default;
  virtual ~IInputHandler() = default;

  virtual void KeyCallback();
  virtual void CharCallback();
  virtual void CharModsCallback();
  virtual void MouseButtonCallback();
  virtual void CursorPosCallback();
  virtual void CursortEnterCallback();
  virtual void ScrollCallback();
  virtual void DropCallback();
};

class IWindowInputHandler {
 public:
  IWindowInputHandler() = default;
  virtual ~IWindowInputHandler() = default;

  virtual void WindowPosCallback();
  virtual void WindowSizeCallback();
  virtual void WindowCloseCallback();
  virtual void WindowRefreshCallback();
  virtual void WindowFocusCallback();
  virtual void WindowIconifyCallback();
  virtual void WindowMaximizeCallback();
  virtual void FramebufferSizeCallback();
  virtual void WindowContentScaleCallback();
};



}

#endif