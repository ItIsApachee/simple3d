#include <simple3d/graphics/models/cuboid.h>

#include <cstddef>
#include <array>
#include <chrono>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <simple3d/graphics/scene.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/models/model_shader.h>
#include <simple3d/graphics/internal/element_buffer_object.h>
#include <simple3d/graphics/internal/vertex_buffer_object.h>
#include <simple3d/graphics/internal/vertex_array_object.h>
#include <simple3d/graphics/internal/vertex.h>

namespace Simple3D {



static constexpr auto GenVertices() {
  constexpr int faces_cnt = 6;
  constexpr int vertices_per_face = 4;
  constexpr int vertices_cnt = faces_cnt * vertices_per_face;

  std::array<Internal::Vertex, vertices_cnt> result{};

  std::array<glm::vec3, 3> axis = {
    glm::vec3{1.f, 0.f, 0.f},
    glm::vec3{0.f, 1.f, 0.f},
    glm::vec3{0.f, 0.f, 1.f}
  };

  // 6 {normal, axis} pairs for faces
  // normal is collinear with axis
  std::array<std::pair<glm::vec3, int>, 6> faces = {
    std::pair{axis[0], 0},
    {-axis[0], 0},
    {axis[1], 1},
    {-axis[1], 1},
    {axis[2], 2},
    {-axis[2], 2},
  };

  for (int i = 0; i < faces_cnt; i++) {
    auto [normal, axis_id] = faces[i];
    int shift_axis0_id = 0 + axis_id == 0;
    int shift_axis1_id = shift_axis0_id + 1 + (axis_id == shift_axis0_id + 1);
    glm::vec3 shift_axis0 = axis[shift_axis0_id];
    glm::vec3 shift_axis1 = axis[shift_axis1_id];

    result[vertices_per_face*i+0] = Internal::Vertex{normal + shift_axis0 + shift_axis1, normal};
    result[vertices_per_face*i+1] = Internal::Vertex{normal + shift_axis0 - shift_axis1, normal};
    result[vertices_per_face*i+2] = Internal::Vertex{normal - shift_axis0 - shift_axis1, normal};
    result[vertices_per_face*i+3] = Internal::Vertex{normal - shift_axis0 + shift_axis1, normal};
  }
  
  for (int i = 0; i < vertices_cnt; i++) {
    result[i].texture_coords = glm::vec2(0.0f);
    result[i].texture_coords.x = (i / vertices_per_face + 1) / 7.f;
    result[i].position /= 2.f;
  }

  return result;
}
static constexpr auto GenIndices() {
  constexpr int faces_cnt = 6;
  constexpr int vertices_per_face = 4;
  constexpr int indices_per_face = 6;
  constexpr int indices_cnt = faces_cnt * indices_per_face;

  std::array<GLuint, indices_cnt> indices{};
  for (int i = 0; i < faces_cnt; i++) {
    indices[indices_per_face*i+0] = vertices_per_face*i+0;
    indices[indices_per_face*i+1] = vertices_per_face*i+1;
    indices[indices_per_face*i+2] = vertices_per_face*i+2;
    indices[indices_per_face*i+3] = vertices_per_face*i+2;
    indices[indices_per_face*i+4] = vertices_per_face*i+3;
    indices[indices_per_face*i+5] = vertices_per_face*i+0;
  }

  return indices;
}
static constexpr auto kCuboidVertices = GenVertices();
static constexpr auto kCuboidIndices = GenIndices();
// static constexpr std::array<Internal::Vertex, 4> kCuboidVertices = {
//   Internal::Vertex{{0.5f, 0.5f, 0.0f}},
//   {{0.5f, -0.5f, 0.0f}},
//   {{-0.5f, -0.5f, 0.0f}},
//   {{-0.5f, 0.5f, 0.0f}}
// };
// static constexpr std::array<GLuint, 3> kCuboidIndices = {
//   0, 1, 2
// };

// static constexpr float vertices[] = ;
CuboidRenderer::CuboidRenderer()
    : vao_(),
      vbo_(),
      ebo_(),
      cuboids_{},
      start_time_{std::chrono::high_resolution_clock::now()} {

  std::cerr << "debug: " << std::endl;
  for (auto vertex: kCuboidVertices) {
    std::cerr << "normal: " << vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z;
    std::cerr << ", position: " << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z;
    std::cerr << std::endl;
  }
  std::cerr << "debug: " << std::endl;
  for (auto index : kCuboidIndices) {
    std::cerr << index << std::endl;
  }

  ModelShader::Init();
  // float vertices[] = {
  //   0.5f,  0.5f, 0.0f,  // top right
  //   0.5f, -0.5f, 0.0f,  // bottom right
  //   -0.5f, -0.5f, 0.0f,  // bottom left
  //   -0.5f,  0.5f, 0.0f   // top left 
  // };

  // float vertices[] = {
  //   // positions         // colors
  //   0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
  //   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
  //   0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
  // };
  // GLuint indices[] = {0, 1, 2};

  vao_.Bind();

  // vbo_ = Internal::VertexBufferObject(sizeof(vertices), 
  //     reinterpret_cast<std::byte*>(vertices));
  vbo_ = Internal::VertexBufferObject(sizeof(Internal::Vertex)*kCuboidVertices.size(), 
      (std::byte*)kCuboidVertices.data());
  vbo_.Bind();
  Internal::Vertex::BindAttributes();
  
  // ebo_ = Internal::ElementBufferObjectBuilder()
  //   .Data(sizeof(indices), reinterpret_cast<std::byte*>(indices))
  //   .Build(vao_);
  ebo_ = Internal::ElementBufferObjectBuilder()
    .Data(sizeof(GLuint)*kCuboidIndices.size(), (std::byte*)kCuboidIndices.data())
    .Build(vao_);
  vao_.BindEbo(ebo_);
  vao_.Unbind();

  // position_vbo_ = 

  // glVertexAttribPointer(
  //     0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(
  //     1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);
}

CuboidRenderer::~CuboidRenderer() {
  for (auto cuboid : cuboids_) {
    delete cuboid;
  }
}

Cuboid* CuboidRenderer::Create(float x, float y, float z) {
  cuboids_.push_back(new Cuboid{x, y, z});
  return cuboids_.back();
}

void CuboidRenderer::Draw() {
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::high_resolution_clock::now() - start_time_);
  
  glm::mat4 transform(1.);
  transform = glm::rotate(transform, elapsed.count() / 1000.f, glm::normalize(glm::vec3{.1, .2, .4}));

  // std::cerr << "drawing cuboid" << std::endl;

  ModelShader::Use();
  vao_.Bind();

  // GLuint model_location = glGetUniformLocation(ModelShader::GetInstance().shader().GetID(), "model");
  ModelShader::GetInstance().shader().SetUniformMat4fv("model", transform);

  // glDrawArrays(gl_dra, 0, 3);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
  vao_.Unbind();
}

void CuboidRenderer::Destroy(Scene* scene) {
  Internal::RendererStorage<CuboidRenderer>.erase(scene);
}



}