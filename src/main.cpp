#include <iostream>
#include <thread>
#include <cmath>
#include <GLFW/glfw3.h>

#define GLAD_GLES2_IMPLEMENTATION
#include <glad/gles2.h>

void error_callback(int error, const char* description) {
    std::cerr << "glfw error: " << error << " with message \"" << description << "\"" << std::endl;
}

void render(GLFWwindow* window) {
    glfwMakeContextCurrent(window);

    GladGLES2Context gl;
    int version = gladLoadGLES2Context(&gl, glfwGetProcAddress);
    if (version == 0) {
        std::cerr << "glad: loading gles2 failed" << std::endl;
        return;
    }
    std::cout << "successfuly loaded GLES2 v" << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    float v = 0.0f;
    const float PI = acos(-1.0);

    gl.Viewport(0, 0, 640, 480);

    while (!glfwWindowShouldClose(window)) {
        gl.ClearColor((sin(v)+1.)/(2*PI), 0.0f, 0.0f, 1.0f);
        gl.Clear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);

        v += 0.1f;
    }
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

    std::thread render_thread(render, window);
    render_thread.detach();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    return 0;
}