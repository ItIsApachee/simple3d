#include <simple3d/graphics/models/cuboid.h>

#include <cstddef>
#include <iostream>
#include <array>
// #include <chrono>
#include <utility>

// FIXME: don't include everything
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <simple3d/graphics/scene.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/models/model_shader.h>
#include <simple3d/graphics/internal/element_buffer_object.h>
#include <simple3d/graphics/internal/vertex_buffer_object.h>
#include <simple3d/graphics/internal/vertex_array_object.h>
#include <simple3d/graphics/internal/vertex.h>

namespace Simple3D {



static constexpr GLenum kInstanceVboUsage = GL_DYNAMIC_DRAW;

struct CuboidInstance {
  static void BindAttributes();
  glm::mat4 model;
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
};

void CuboidInstance::BindAttributes() {
  constexpr auto model_layout_index = 2;
  constexpr auto model_row_size = sizeof(glm::vec4);
  constexpr auto model_rows = 4;

  for (int i = 0; i < model_rows; i++) {
    glVertexAttribPointer(
        model_layout_index + i, 4, GL_FLOAT, GL_FALSE,
        sizeof(CuboidInstance), reinterpret_cast<void*>(
          offsetof(CuboidInstance, model) + model_row_size * i
        ));
    glEnableVertexAttribArray(model_layout_index + i);
    glVertexAttribDivisor(model_layout_index + i, 1);
  }

  constexpr auto diffuse_color_layout_index = 10;
  glVertexAttribPointer(
      diffuse_color_layout_index, 3, GL_FLOAT, GL_FALSE,
      sizeof(CuboidInstance),
      reinterpret_cast<void*>(offsetof(CuboidInstance, diffuse_color)));
  glEnableVertexAttribArray(diffuse_color_layout_index);
  glVertexAttribDivisor(diffuse_color_layout_index, 1);

  constexpr auto specular_color_layout_inedx = 11;
  glVertexAttribPointer(
    specular_color_layout_inedx, 3, GL_FLOAT, GL_FALSE,
    sizeof(CuboidInstance),
    reinterpret_cast<void*>(offsetof(CuboidInstance, specular_color)));

}

static constexpr auto GenVertices() {
  // TODO(apachee): fix order for face culling to work

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

CuboidRenderer::CuboidRenderer()
    : vao_(),
      verices_vbo_(),
      instances_vbo_(),
      instances_vbo_capacity_{0},
      ebo_(),
      cuboids_(),
      start_time_(std::chrono::high_resolution_clock::now()) {
  vao_.Bind();

  verices_vbo_ = Internal::VertexBufferObject(sizeof(Internal::Vertex)*kCuboidVertices.size(), 
      (const std::byte*)(kCuboidVertices.data()));
  verices_vbo_.Bind();
  Internal::Vertex::BindAttributes();

  instances_vbo_ = Internal::VertexBufferObject(0, nullptr, kInstanceVboUsage);
  instances_vbo_.Bind();
  CuboidInstance::BindAttributes();
  
  ebo_ = Internal::ElementBufferObjectBuilder()
    .Data(sizeof(GLuint)*kCuboidIndices.size(), (const std::byte*)(kCuboidIndices.data()))
    .Build(vao_);
  vao_.BindEbo(ebo_);
  vao_.Unbind();

  std::cerr << "vertices: " << std::endl;
  for (auto vertex: kCuboidVertices) {
    std::cerr << vertex.position.x << ' ' << vertex.position.y << ' ' << vertex.position.z << std::endl;
  }

  std::cerr << "indices: " << std::endl;
  for (auto index: kCuboidIndices) {
    std::cerr << index << std::endl;
  }
}

CuboidRenderer::~CuboidRenderer() {
  for (auto cuboid : cuboids_) {
    delete cuboid;
  }
}

void CuboidRenderer::Draw() {
  // std::cout << "Draw()" << std::endl;
  if (cuboids_.empty()) {
    // std::cout << "wtf" << std::endl;
    return;
  }

  if (cuboids_.size() > instances_vbo_capacity_) {
    instances_vbo_.SetData(cuboids_.size()*sizeof(CuboidInstance), nullptr, kInstanceVboUsage);
    instances_vbo_capacity_ = cuboids_.size();
  }

  // auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
  //   std::chrono::high_resolution_clock::now() - start_time_);
  
  // glm::mat4 transform(1.0f);
  // transform = glm::rotate(transform, elapsed.count() / 1000.f, glm::normalize(glm::vec3{.1, .2, .4}));
  
  std::size_t instances_cnt = cuboids_.size();

  std::vector<CuboidInstance> instances;
  instances.reserve(instances_cnt);
  // std::cerr << "debug: " << std::endl;

  for (const auto cuboid_ptr: cuboids_) {
    auto& cuboid = *cuboid_ptr;

    glm::mat4 model(1.0f);
    glm::vec3 pos(cuboid.pos);
    model = glm::translate(model, pos);
    // model = model * transform;

    // glm::vec3 color(cuboid.r, cuboid.g, cuboid.b);

    // std::cerr << "Cube({" << std::endl;
    // for (int i = 0; i < 4; i++) {
    //   std::cerr << '\t';
    //   for (int j = 0; j < 4; j++) {
    //     std::cerr << model[i][j] << ", ";
    //   }
    //   std::cerr << std::endl;
    // }
    // std::cerr << std::endl << "}, {" << color.x << ", " << color.y << ", " << color.z << "}" << std::endl << ")" << std::endl;

    instances.push_back(CuboidInstance{model,
        cuboid.diffuse_color, cuboid.specular_color});
  }
  instances_vbo_.SubData(0, instances_cnt * sizeof(CuboidInstance),
      (const std::byte*)instances.data());


  // CuboidInstance inst{transform};
  // instances_vbo_.SubData(0, sizeof(CuboidInstance), (std::byte*)&inst);
  vao_.Bind();


  // ModelShader::GetInstance().shader().SetUniformMat4fv("model", transform);

  // ModelShader::GetInstance().shader().SetUniformMat4fv("model_test", transform);

  // glm::mat4 view(1.0f);
  // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -350.0f));
  // view = glm::rotate(view, elapsed.count() / 25000.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  // view = glm::rotate(view, elapsed.count() / 4000.0f, glm::vec3(0.0f, 1.0f, 0.0f));

  // ModelShader::GetInstance().shader().SetUniformMat4fv("view", view);

  // glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 1000.0f);
  // glm::mat4 projection(1.0f);
  // ModelShader::GetInstance().shader().SetUniformMat4fv("projection", projection);

  // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, static_cast<void*>(0));
  // std::cerr << "instances_cnt: " << instances_cnt << std::endl;
  // vao_.BindEbo(ebo_);
  glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0, static_cast<GLsizei>(instances_cnt));
  vao_.Unbind();
}

void CuboidRenderer::Destroy(void* cuboid_void) {
  Cuboid* cuboid_ptr = reinterpret_cast<Cuboid*>(cuboid_void);
  cuboids_.erase(cuboid_ptr);
}



}