#ifndef INCLUDE_SIMPLE3D_IMGUI_IMGUI_H_
#define INCLUDE_SIMPLE3D_IMGUI_IMGUI_H_

namespace Simple3D::ImGui {



void CreateContext();
void NewFrame();
void Render();
void DestroyContext();

class ImGuiContext {
 public:
  static ImGuiContext& GetInstance();

  ImGuiContext(const ImGuiContext&) = delete;
  ImGuiContext(ImGuiContext&&) = delete;
  ImGuiContext& operator=(const ImGuiContext&) = delete;
  ImGuiContext& operator=(ImGuiContext&&) = delete;
  ~ImGuiContext() = default;

  void CreateContext();
  void NewFrame();
  void Render();
  void DestroyContext();

 private:
  ImGuiContext() = default;
};



}  // namespace Simple3D::ImGui

#endif  // INCLUDE_SIMPLE3D_IMGUI_IMGUI_H_
