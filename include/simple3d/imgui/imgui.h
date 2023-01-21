#ifndef INCLUDE_SIMPLE3D_IMGUI_IMGUI_H_
#define INCLUDE_SIMPLE3D_IMGUI_IMGUI_H_

#include <simple3d/context/input.h>

#include <memory>
#include <unordered_set>

namespace Simple3D::ImGui {

void CreateContext();
void NewFrame();
void Render();
void DestroyContext();

class Context {
 public:
  static Context& GetInstance();

  Context(const Context&) = delete;
  Context(Context&&) = delete;
  Context& operator=(const Context&) = delete;
  Context& operator=(Context&&) = delete;
  ~Context() = default;

  void CreateContext();
  void NewFrame();
  void Render();
  void DestroyContext();

 private:
  Context() = default;
};

class InputHandler : public IInputHandler, public IWindowInputHandler {
 public:
  InputHandler();
  InputHandler(const InputHandler&) = delete;
  InputHandler(InputHandler&&) = default;
  InputHandler& operator=(const InputHandler&) = delete;
  InputHandler& operator=(InputHandler&&) = default;
  ~InputHandler() override = default;

  void ToggleInputs(bool enable);

  void EnableInputHandler(const std::shared_ptr<IInputHandler>&);
  void EnableWindowInputHandler(const std::shared_ptr<IWindowInputHandler>&);
  void DisableInputHandler(const std::shared_ptr<IInputHandler>&);
  void DisableWindowInputHandler(const std::shared_ptr<IWindowInputHandler>&);

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
  GLFWwindow* window{nullptr};
  bool enabled{true};

  std::unordered_set<std::shared_ptr<IInputHandler>> input_handlers_{};
  std::unordered_set<std::shared_ptr<IWindowInputHandler>>
      window_input_handlers_{};
};

}  // namespace Simple3D::ImGui

#endif  // INCLUDE_SIMPLE3D_IMGUI_IMGUI_H_
