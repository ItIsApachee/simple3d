#include <iostream>
#include <GLFW/glfw3.h>

#define GLAD_GLES2_IMPLEMENTATION
#include <glad/gles2.h>

void error_callback(int error, const char* description) {
    std::cerr << "glfw error: " << error << " with message \"" << description << "\"" << std::endl;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "glfw initialization failed" << std::endl;
        return 1;
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    GLFWwindow* window = glfwCreateWindow(640, 480, "My title", nullptr, nullptr);
    if (!window) {
        std::cerr << "window creation failed" << std::endl;
        return 2;
    }
    glfwMakeContextCurrent(window);

    GladGLES2Context gl;
    int version = gladLoadGLES2Context(&gl, glfwGetProcAddress);
    if (version == 0) {
        std::cerr << "glad: loading gles2 failed" << std::endl;
        return 3;
    }

    gl.Viewport(0, 0, 640, 480);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        gl.ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl.Clear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    return 0;
}