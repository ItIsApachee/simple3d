#include <simple3d/context/context.h>

#include <iostream>
#include <string>
#include <memory>

#include <glad/gles2.h>
#include <GLFW/glfw3.h>

#include <simple3d/misc/error.h>
#include <simple3d/context/window.h>

namespace Simple3D {



// FIXME: provide better error handling
static void error_callback(int code, const char* description) {
    std::cerr << "GLFW error: 0x" << std::hex << code << ": " << description << std::endl;
}

Context& Context::GetInstance() {
    static Context main_loop = Context();
    return main_loop;
}

Context::Context() {}

Error Context::Init() {
    Context& ctx = GetInstance();
    if (ctx.is_init) {
        return Error(ErrorType::kInitFailed, "already initialized");
    }
    int glfw_init_error = glfwInit();
    if (glfw_init_error == GLFW_FALSE) {
        return Error(ErrorType::kInitFailed, "glfw initialization failed");
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // doesn't work with ANGLE for some reason
    // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE); // disable vsync

    GLFWwindow* glfw_window = glfwCreateWindow(500, 500, "test (FIXME)", nullptr, nullptr);
    if (glfw_window == nullptr) {
        return Error(ErrorType::kInitFailed, "glfw failed to create window");
    }

    ctx.window_ = Window::Create(glfw_window);

    // TODO: Load GLES context
    // TODO: setup callbacks(?): inputs, etc.

    ctx.is_init = true;

    return Error::Ok();
}

void Context::Destroy() {
    Context& ctx = GetInstance();
    
    if (ctx.is_init) {
        ctx.window_->Destroy();
        glfwTerminate();
        ctx.is_init = false;
    }
}

void Context::PollEvents() {
    glfwPollEvents();
}

std::shared_ptr<Window> Context::GetWindow() {
    return GetInstance().window_;
}



}