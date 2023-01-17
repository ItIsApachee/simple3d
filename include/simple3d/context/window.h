/**
 * \file
 * \brief Window handling
*/

#ifndef INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_
#define INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_

#include <unordered_set>
#include <memory>

#include <GLFW/glfw3.h>

#include <simple3d/types.h>
#include <simple3d/context/input.h>

namespace Simple3D {



class Window : public IInputHandler, public IWindowInputHandler {
 public:
  friend App;

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  ~Window() override;

  void KeyCallback(int key, int scancode, int action, int mods) override;
  void CharCallback(unsigned int codepoint) override;
  void CharModsCallback(unsigned int codepoint, int mods) override;
  void MouseButtonCallback(int button, int action, int mods) override;
  void CursorPosCallback(double xpos, double ypos) override;
  void CursorEnterCallback(int entered) override;
  void ScrollCallback(double xoffset, double yoffset) override;
  void DropCallback(int path_count, const char* paths[]) override;

  void WindowPosCallback(int xpos, int ypos) override;
  void WindowSizeCallback(int width, int height) override;
  void WindowCloseCallback() override;
  void WindowRefreshCallback() override;
  void WindowFocusCallback(int focused) override;
  void WindowIconifyCallback(int iconified) override;
  void WindowMaximizeCallback(int maximized) override;
  void FramebufferSizeCallback(int width, int height) override;
  void WindowContentScaleCallback(float xscale, float yscale) override;

 private:
  static Window Create(Error* error);

  Window() = default;
  explicit Window(GLFWwindow* window);
  Window(Window&&);
  Window& operator=(Window&&);

  void LoadGLES2();

  bool ShouldClose();
  void SwapBuffers();
  void EnableInputHandler(const std::shared_ptr<IInputHandler>& input_handler);
  void EnableWindowInputHandler(
    const std::shared_ptr<IWindowInputHandler>& window_input_handler);
  void DisableInputHandler(const std::shared_ptr<IInputHandler>& input_handler);
  void DisableWindowInputHandler(
    const std::shared_ptr<IWindowInputHandler>& window_input_handler);

  GLFWwindow* window_{nullptr};
  std::unordered_set<std::shared_ptr<IInputHandler>> input_handlers_{};
  std::unordered_set<std::shared_ptr<IWindowInputHandler>> window_input_handlers_{};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_
