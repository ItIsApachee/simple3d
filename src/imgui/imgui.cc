#include <simple3d/imgui/imgui.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui-gles3/imgui_impl_gles3.h>
#include <GLFW/glfw3.h>

#include <simple3d/context/context.h>

// can't call macro in Simple3D::ImGui
// because of the ImGui part of the namespace
static void check_version() {
  IMGUI_CHECKVERSION();
}

namespace Simple3D::ImGui {

void CreateContext() {
  ImGuiContext::GetInstance().CreateContext();
}

void NewFrame() {
  ImGuiContext::GetInstance().NewFrame();
}

void Render() {
  ImGuiContext::GetInstance().Render();
}

void DestroyContext() {
  ImGuiContext::GetInstance().DestroyContext();
}

ImGuiContext& ImGuiContext::GetInstance() {
  static ImGuiContext ctx{};
  return ctx;
}

void ImGuiContext::CreateContext() {
  ::check_version();

  ::ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(App::GetGLFWwindow(), false);
  ImGui_ImplGles3_Init();
}

void ImGuiContext::NewFrame() {
  ImGui_ImplGles3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ::ImGui::NewFrame();
}

void ImGuiContext::Render() {
  ::ImGui::Render();
  ImGui_ImplGles3_RenderDrawData(::ImGui::GetDrawData());
}

void ImGuiContext::DestroyContext() {
  ImGui_ImplGles3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ::ImGui::DestroyContext();
}

}  // namespace Simple3D::ImGui
