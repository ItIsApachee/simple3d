#include <iostream>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description) {
    std::cerr << "glfw error: " << error << " with message \"" << description << "\"" << std::endl;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "glfw initialization failed" << std::endl;
        return 1;
    }
    glfwSetErrorCallback(error_callback);

    GLFWwindow* window = glfwCreateWindow(640, 480, "My title", nullptr, nullptr);
    if (!window) {
        std::cerr << "window creation failed" << std::endl;
        return 2;
    }
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    return 0;
}