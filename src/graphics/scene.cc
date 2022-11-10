#include <simple3d/graphics/scene.h>

#include <cmath>

#include <memory>
#include <random>
#include <utility>
#include <chrono>

#define GLAD_GLES2_IMPLEMENTATION
#include <glad/gles2.h>
#include <GLFW/glfw3.h>

namespace Simple3D {



void render_scene(std::shared_ptr<Scene> scene) {
    GLFWwindow* glfw_window = scene->window_->GetGLFWwindow();
    glfwMakeContextCurrent(glfw_window);

    GladGLES2Context gl;
    int version = gladLoadGLES2Context(&gl, glfwGetProcAddress);

    auto start = std::chrono::high_resolution_clock::now();

    std::uniform_real_distribution<float> distrib(0.0f, 1.0f);
    std::mt19937 rnd(1337);
    float colors[2] = {
        distrib(rnd), distrib(rnd)
    };
    while (scene->is_running_.test_and_set()) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = (now - start).count();
        float v = elapsed / (float)1e9 * 10;

        gl.ClearColor(colors[0], colors[1], (sin(v)+1)/2.0f, 1.0f);
        gl.Clear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(glfw_window);
    }
}

Scene::Scene() {
    is_running_.test_and_set();
}

void Scene::SetContext(std::shared_ptr<Window> window) {
    if (window_) {
        is_running_.clear();
        if (render_thread_.joinable())
            render_thread_.join();
    }
    window_ = std::move(window);
    render_thread_ = std::thread(render_scene, shared_from_this());
}

void Scene::ClearContext() {
    if (window_) {
        is_running_.clear();
        if (render_thread_.joinable())
            render_thread_.join();
    }
}

std::shared_ptr<Scene> Scene::Create() {
    auto scene = std::shared_ptr<Scene>(
        new Scene()
    );
    return scene;
}



}