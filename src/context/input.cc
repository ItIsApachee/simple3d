#include <simple3d/context/input.h>

namespace Simple3D {



void IInputHandler::KeyCallback() {}
void IInputHandler::CharCallback() {}
void IInputHandler::CharModsCallback() {}
void IInputHandler::MouseButtonCallback() {}
void IInputHandler::CursorPosCallback() {}
void IInputHandler::CursortEnterCallback() {}
void IInputHandler::ScrollCallback() {}
void IInputHandler::DropCallback() {}

void IWindowInputHandler::WindowPosCallback() {}
void IWindowInputHandler::WindowSizeCallback() {}
void IWindowInputHandler::WindowCloseCallback() {}
void IWindowInputHandler::WindowRefreshCallback() {}
void IWindowInputHandler::WindowFocusCallback() {}
void IWindowInputHandler::WindowIconifyCallback() {}
void IWindowInputHandler::WindowMaximizeCallback() {}
void IWindowInputHandler::FramebufferSizeCallback() {}
void IWindowInputHandler::WindowContentScaleCallback() {}



}  // namespace Simple3D