#include <simple3d/context/context.h>

#include <iostream>
#include <string>
#include <memory>

#include <GLFW/glfw3.h>

#include <simple3d/misc/error.h>

namespace Simple3D {



MainLoop& MainLoop::GetInstance() {
    static MainLoop main_loop = MainLoop();
    return main_loop;
}

static void error_callback(int code, const char* description) {
    std::cerr << "GLFW error: 0x" << std::hex << code << ": " << description << std::endl;
}

Error MainLoop::Init() {
    int glfw_init_error = glfwInit();
    if (glfw_init_error == GLFW_FALSE) {
        return Error(ErrorType::kInitFailed, "glfw initialization failed");
    }
    glfwSetErrorCallback(error_callback);

    // TODO: setup callbacks: errors, inputs, etc.

    return Error::Ok();
}

Error MainLoop::Start() {
    // TODO: add stop method
    while (true) {
        glfwPollEvents();
        
        // render windows
    }
}

void MainLoop::Destroy() {
    glfwTerminate();
}

void MainLoop::AddWindow(std::shared_ptr<Window> window) {
    windows_.insert(window);
}

std::shared_ptr<Window> WindowBuilder::Build() {
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // doesn't work with ANGLE for some reason
    // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE); // disable vsync

    GLFWwindow* glfw_window = glfwCreateWindow(500, 500, title_.c_str(), nullptr, nullptr);

    std::shared_ptr<Window> window;
    if (glfw_window != nullptr) {
        window.reset(new Window(glfw_window));
        MainLoop::GetInstance().AddWindow(window);
    }
    return window;
}

WindowBuilder& WindowBuilder::Title(std::string title) {
    title_ = title;
    return *this;
}

GLFWwindow* Window::GetGLFWwindow() const {
    return glfw_window_;
}



}