#include <simple3d/graphics/scene.h>

#include <cmath>

#include <iostream>
#include <memory>
#include <random>
#include <utility>
#include <chrono>

#include <glad/gles2.h>
#include <GLFW/glfw3.h>

#include <simple3d/shader/shader.h>
#include <simple3d/shader/shader_source.h>

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

    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    int success;
    char info_log[512];

    unsigned int vbo = 0;
    gl.GenBuffers(1, &vbo);
    gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
    gl.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Shader shader_program = ShaderBuilder().VertexShaderSource(kVertexShader).FragmentShaderSource(kFragmentShader).Build(&gl);

    // gl.UseProgram(shader_program);
    // gl.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // gl.EnableVertexAttribArray(0);

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    unsigned int ebo;
    gl.GenBuffers(1, &ebo);

    unsigned int vao;
    gl.GenVertexArrays(1, &vao);  
    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    gl.BindVertexArray(vao);
    // 2. copy our vertices array in a buffer for OpenGL to use
    gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
    gl.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // 3. then set our vertex attributes pointers
    gl.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    gl.EnableVertexAttribArray(0);  

    // ..:: Drawing code (in render loop) :: ..
    // 4. draw the object
    // glUseProgram(shaderProgram);
    // glBindVertexArray(VAO);
    // someOpenGLFunctionThatDrawsOurTriangle();   

    while (scene->is_running_.test_and_set()) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = (now - start).count();
        float v = elapsed / (float)1e9 * 10;

        gl.ClearColor(colors[0], colors[1], (sin(v)+1)/2.0f, 1.0f);
        gl.Clear(GL_COLOR_BUFFER_BIT);

        // gl.UseProgram(shader_program);
        shader_program.Use(&gl);
        gl.BindVertexArray(vao);
        // gl.DrawArrays(GL_TRIANGLES, 0, 3);
        gl.DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
        gl.BindVertexArray(0);

        gl.Flush();
        // someOpenGLFunctionThatDrawsOurTriangle();   

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