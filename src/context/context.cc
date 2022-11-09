#include <simple3d/context/context.h>

#include <string>

#include <GLFW/glfw3.h>

#include <simple3d/misc/error.h>

namespace Simple3D {

MainLoop& MainLoop::GetInstance() {
    static MainLoop main_loop;
    return main_loop;
}


Error MainLoop::Init() {
    int glfw_init_error = glfwInit();
    if (glfw_init_error == GLFW_FALSE) {
        return Error(ErrorType::kInitFailed, "glfw initialization failed");
    }

    // TODO: setup callbacks

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

}