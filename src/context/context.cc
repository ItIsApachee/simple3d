#include <GLFW/glfw3.h>
#include <glad/gles2.h>
#include <simple3d/context/context.h>
#include <simple3d/context/window.h>
#include <simple3d/graphics/internal/gles_shader.h>
#include <simple3d/graphics/internal/misc.h>
#include <simple3d/misc/error.h>

#include <iostream>
#include <memory>
#include <string>

namespace Simple3D {

// FIXME(apachee): provide better error handling
static void error_callback(int code, const char* description) {
  std::cerr << "GLFW error: 0x" << std::hex << code << ": " << description;
  std::cerr << std::endl;
}

App& App::GetInstance() {
  static App main_loop = App();
  return main_loop;
}

// App::App() {}

Error App::Init() {
  App& ctx = GetInstance();
  if (ctx.ctx_id_ != 0) {
    return Error(ErrorType::kInitFailed, "already initialized");
  }
  int glfw_init_error = glfwInit();
  if (glfw_init_error == GLFW_FALSE) {
    return Error(ErrorType::kInitFailed, "glfw initialization failed");
  }
  glfwSetErrorCallback(error_callback);

  // std::cout << "initliazed glfw successfully" << std::endl;

  Error err{};
  ctx.window_ = Window::Create(&err);
  if (!err.IsOk()) {
    return err;
  }
  Internal::SetInputHandler(ctx.window_.window_, &ctx.window_);
  Internal::SetWindowInputHandler(ctx.window_.window_, &ctx.window_);

  // std::cerr << "window creation succ" << std::endl;

  ctx.last_ctx_id_ += 1;
  ctx.ctx_id_ = ctx.last_ctx_id_;

  return Error::Ok();
}

void App::Destroy() {
  App& ctx = GetInstance();

  if (ctx.ctx_id_ != 0) {
    Internal::UnsetInputHandler();
    Internal::UnsetWindowInputHandler();

    Internal::GlesShader::active_shader_id_ = Internal::kGlesInvalidShader;
    {
      Window window = std::move(ctx.window_);
      ctx.window_ = Window{};
      // window is destroyed once out of scope
    }
    glfwTerminate();
    ctx.ctx_id_ = 0;
  }
}

bool App::ShouldClose() { return GetInstance().window_.ShouldClose(); }

void App::SwapBuffers() { GetInstance().window_.SwapBuffers(); }

void App::PollEvents() { glfwPollEvents(); }

void App::EnableInputHandler(std::shared_ptr<IInputHandler> input_handler) {
  GetInstance().window_.EnableInputHandler(input_handler);
}
void App::EnableWindowInputHandler(
    std::shared_ptr<IWindowInputHandler> window_input_handler) {
  GetInstance().window_.EnableWindowInputHandler(window_input_handler);
}
void App::DisableInputHandler(std::shared_ptr<IInputHandler> input_handler) {
  GetInstance().window_.DisableInputHandler(input_handler);
}
void App::DisableWindowInputHandler(
    std::shared_ptr<IWindowInputHandler> window_input_handler) {
  GetInstance().window_.DisableWindowInputHandler(window_input_handler);
}

GLFWwindow* App::GetGLFWwindow() { return GetInstance().window_.window_; }

std::int64_t App::GetCtxId() { return GetInstance().ctx_id_; }

}  // namespace Simple3D
