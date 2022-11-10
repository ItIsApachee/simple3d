#include <simple3d/graphics/scene.h>

#include <memory>
// tmp
#include <tuple>

#define GLAD_GLES2_IMPLEMENTATION
#include <glad/gles2.h>
#include <GLFW/glfw3.h>

namespace Simple3D {



void render_scene(std::shared_ptr<Scene> scene) {
    GLFWwindow* glfw_window = scene->window_->GetGLFWwindow();
    glfwMakeContextCurrent(glfw_window);

    GladGLES2Context gl;
    int version = gladLoadGLES2Context(&gl, glfwGetProcAddress);

    while (scene->is_running_.test_and_set()) {
        gl.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        gl.Clear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(glfw_window);
    }
}

Scene::Scene(std::shared_ptr<Window> window): window_{window} {
    is_running_.test_and_set();
}

std::shared_ptr<Scene> Scene::Create(std::shared_ptr<Window> window) {
    auto scene = std::shared_ptr<Scene>(
        new Scene(window)
    );
    scene->render_thread_ = std::thread(render_scene, scene);
    return scene;
}



}