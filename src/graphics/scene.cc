#include <simple3d/graphics/scene.h>

#include <cmath>

#include <iostream>
#include <memory>
#include <utility>
// #include <chrono>

#include <glad/gles2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <simple3d/graphics/camera.h>
// #include <simple3d/shader/shader.h>
// #include <simple3d/shader/shader_source.h>

namespace Simple3D {



Scene::Scene() : renderers_{}, active_camera_{new ICamera()} {}

void Scene::SetCamera(std::shared_ptr<ICamera> camera) {
  if (camera) {
    active_camera_ = camera;
  } else {
    active_camera_ = std::shared_ptr<ICamera>(new ICamera());
  }
}
/*
// void render_scene(std::shared_ptr<Scene> scene) {
//     GladGLES2Context gl = scene->window_->GetGLES2Context();

//     auto start = std::chrono::high_resolution_clock::now();

//     // float vertices[] = {
//     //     0.5f,  0.5f, 0.0f,  // top right
//     //     0.5f, -0.5f, 0.0f,  // bottom right
//     //     -0.5f, -0.5f, 0.0f,  // bottom left
//     //     -0.5f,  0.5f, 0.0f   // top left 
//     // };

//     Error shader_program_error;
//     Shader shader_program = ShaderBuilder()
        .VertexShaderSource(kVertexShader)
        .FragmentShaderSource(kFragmentShader).Build(
          &gl, shader_program_error);
//     if (!shader_program.IsValid() || !shader_program_error.IsOk()) {
//         std::cerr << "bad shader program: "
            << shader_program_error.description
            std::cerr << std::endl;
//         return;
//     }

//     float vertices[] = {
//         // positions         // colors
//         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
//         -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
//         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
//     };    

//     unsigned int vao = 0;
//     unsigned int vbo = 0;

//     gl.GenVertexArrays(1, &vao);  
//     gl.GenBuffers(1, &vbo);

//     gl.BindVertexArray(vao);
//     gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
//     gl.BufferData(
        GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


//     gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
//     gl.BufferData(
          GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     gl.VertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//     gl.EnableVertexAttribArray(0);
//     gl.VertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//     gl.EnableVertexAttribArray(1);
//     // gl.VertexAttribPointer(
       //   0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//     // gl.EnableVertexAttribArray(0);  

//     // gl.VertexAttribPointer(
       //   0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//     // gl.EnableVertexAttribArray(1);  

//     // ..:: Drawing code (in render loop) :: ..
//     // 4. draw the object
//     // glUseProgram(shaderProgram);
//     // glBindVertexArray(VAO);
//     // someOpenGLFunctionThatDrawsOurTriangle();   
//     std::vector<int> a = std::vector<int>{1, 2, 3};

//     while (scene->is_running_.test_and_set()) {
//         auto now = std::chrono::high_resolution_clock::now();
//         auto elapsed = (now - start).count();
//         float v = elapsed / (float)1e9 * 10;
//         v = (float)(sin(v/2.)+1.)/(float)2.0;

//         glm::mat4 transform = glm::mat4(1.0f);
//         transform = glm::rotate(
            transform, (float)(v*2.*glm::pi<float>()), glm::vec3(0., 0., 1.));
//         shader_program.SetUniformMat4fv(&gl, "transform", transform);

//         gl.ClearColor(0., 0., 0., 1.0f);
//         gl.Clear(GL_COLOR_BUFFER_BIT);

//         // gl.UseProgram(shader_program);
//         shader_program.Use(&gl);
//         gl.BindVertexArray(vao);
//         gl.DrawArrays(GL_TRIANGLES, 0, 3);
//         // // gl.DrawElements(
           // GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0); 
//         gl.BindVertexArray(0);

//         gl.Flush();

//         scene->window_->SwapBuffers();
//     }
// }

// Scene::Scene() {
    // is_running_.test_and_set();
// }

// void Scene::SetContext(std::shared_ptr<Window> window) {
//     if (window_) {
//         is_running_.clear();
//         if (render_thread_.joinable())
//             render_thread_.join();
//     }
//     window_ = std::move(window);
//     render_thread_ = std::thread(render_scene, shared_from_this());
// }

// void Scene::ClearContext() {
//     if (window_) {
//         is_running_.clear();
//         if (render_thread_.joinable())
//             render_thread_.join();
//     }
// }
*/



}  // namespace Simple3D
