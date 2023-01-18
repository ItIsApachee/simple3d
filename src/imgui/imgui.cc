#include <simple3d/imgui/imgui.h>

#include <memory>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui-gles3/imgui_impl_gles3.h>
#include <GLFW/glfw3.h>

#include <simple3d/context/context.h>
#include <simple3d/context/input.h>

// can't call macro in Simple3D::ImGui
// because of the ImGui part of the namespace
static void check_version() {
  IMGUI_CHECKVERSION();
}

namespace Simple3D::ImGui {

void CreateContext() {
  Context::GetInstance().CreateContext();
}

void NewFrame() {
  Context::GetInstance().NewFrame();
}

void Render() {
  Context::GetInstance().Render();
}

void DestroyContext() {
  Context::GetInstance().DestroyContext();
}

Context& Context::GetInstance() {
  static Context ctx{};
  return ctx;
}

void Context::CreateContext() {
  ::check_version();

  ::ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(App::GetGLFWwindow(), false);

  // FIXME: currently simple3d doesn't have monitor callback
  // and at this moment this allows the following:
  glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);

  ImGui_ImplGles3_Init();
}

void Context::NewFrame() {
  ImGui_ImplGles3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ::ImGui::NewFrame();
}

void Context::Render() {
  ::ImGui::Render();
  ImGui_ImplGles3_RenderDrawData(::ImGui::GetDrawData());
}

void Context::DestroyContext() {
  ImGui_ImplGles3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ::ImGui::DestroyContext();
}

InputHandler::InputHandler() : window{App::GetGLFWwindow()} {}

void InputHandler::ToggleInputs(bool enable) {
  enabled = enable;
}

void InputHandler::EnableInputHandler(const std::shared_ptr<IInputHandler>& input_handler) {
  input_handlers_.insert(input_handler);
}

void InputHandler::EnableWindowInputHandler(
    const std::shared_ptr<IWindowInputHandler>& window_input_handler) {
  window_input_handlers_.insert(window_input_handler);
}

void InputHandler::DisableInputHandler(const std::shared_ptr<IInputHandler>& input_handler) {
  input_handlers_.erase(input_handler);
}

void InputHandler::DisableWindowInputHandler(
    const std::shared_ptr<IWindowInputHandler>& window_input_handler) {
  window_input_handlers_.erase(window_input_handler);
}



void InputHandler::KeyCallback(int key, int scancode, int action, int mods) {
  if (enabled) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (::ImGui::GetIO().WantCaptureKeyboard) {
      return;
    }
  }
  for (auto& input_handler : input_handlers_) {
    input_handler->KeyCallback(key, scancode, action, mods);
  }
}
void InputHandler::CharCallback(unsigned int codepoint) {
  if (enabled) {
    ImGui_ImplGlfw_CharCallback(window, codepoint);
    if (::ImGui::GetIO().WantCaptureKeyboard) {
      return;
    }
  }
  for (auto& input_handler : input_handlers_) {
    input_handler->CharCallback(codepoint);
  }
}
void InputHandler::CharModsCallback(unsigned int codepoint, int mods) {
  for (auto& input_handler : input_handlers_) {
    input_handler->CharModsCallback(codepoint, mods);
  }
}
void InputHandler::MouseButtonCallback(int button, int action, int mods) {
  if (enabled) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    if (::ImGui::GetIO().WantCaptureMouse) {
      return;
    }
  }
  for (auto& input_handler : input_handlers_) {
    input_handler->MouseButtonCallback(button, action, mods);
  }
}
void InputHandler::CursorPosCallback(double xpos, double ypos) {
  if (enabled) {
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    if (::ImGui::GetIO().WantCaptureMouse) {
      return;
    }
  }
  for (auto& input_handler : input_handlers_) {
    input_handler->CursorPosCallback(xpos, ypos);
  }
}
void InputHandler::CursorEnterCallback(int entered) {
  ImGui_ImplGlfw_CursorEnterCallback(window, entered);
  for (auto& input_handler : input_handlers_) {
    input_handler->CursorEnterCallback(entered);
  }
}
void InputHandler::ScrollCallback(double xoffset, double yoffset) {
  if (enabled) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    if (::ImGui::GetIO().WantCaptureMouse) {
      return;
    }
  }
  for (auto& input_handler : input_handlers_) {
    input_handler->ScrollCallback(xoffset, yoffset);
  }
}
void InputHandler::DropCallback(int path_count, const char* paths[]) {
  for (auto& input_handler : input_handlers_) {
    input_handler->DropCallback(path_count, paths);
  }
}



void InputHandler::WindowPosCallback(int xpos, int ypos) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowPosCallback(xpos, ypos);
  }
}
void InputHandler::WindowSizeCallback(int width, int height) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowSizeCallback(width, height);
  }
}
void InputHandler::WindowCloseCallback() {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowCloseCallback();
  }
}
void InputHandler::WindowRefreshCallback() {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowRefreshCallback();
  }
}
void InputHandler::WindowFocusCallback(int focused) {
  ImGui_ImplGlfw_WindowFocusCallback(window, focused);
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowFocusCallback(focused);
  }
}
void InputHandler::WindowIconifyCallback(int iconified) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowIconifyCallback(iconified);
  }
}
void InputHandler::WindowMaximizeCallback(int maximized) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowMaximizeCallback(maximized);
  }
}
void InputHandler::FramebufferSizeCallback(int width, int height) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->FramebufferSizeCallback(width, height);
  }
}
void InputHandler::WindowContentScaleCallback(float xscale, float yscale) {
  for (auto& window_input_handler : window_input_handlers_) {
    window_input_handler->WindowContentScaleCallback(xscale, yscale);
  }
}



}  // namespace Simple3D::ImGui
