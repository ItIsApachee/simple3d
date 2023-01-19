#include <simple3d/graphics/models/cuboid.h>

#include <cstddef>
// FIXME: remove iostream
#include <iostream>
#include <array>

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
  float shininess;
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

  constexpr auto specular_color_layout_index = 11;
  glVertexAttribPointer(
    specular_color_layout_index, 3, GL_FLOAT, GL_FALSE,
    sizeof(CuboidInstance),
    reinterpret_cast<void*>(offsetof(CuboidInstance, specular_color)));
  glEnableVertexAttribArray(specular_color_layout_index);
  glVertexAttribDivisor(specular_color_layout_index, 1);
  
  constexpr auto shininess_layout_index = 12;
  glVertexAttribPointer(
    shininess_layout_index, 1, GL_FLOAT, GL_FALSE,
    sizeof(CuboidInstance),
    reinterpret_cast<void*>(offsetof(CuboidInstance, shininess)));
  glEnableVertexAttribArray(shininess_layout_index);
  glVertexAttribDivisor(shininess_layout_index, 1);
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
      cuboids_() {
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

// CuboidRenderer::~CuboidRenderer() {
//   // for (auto cuboid : cuboids_) {
//   //   delete cuboid.first;
//   // }
// }

void CuboidRenderer::Draw(IShader& shader_) {
  if (cuboids_.empty()) {
    return;
  }

  // resize VBO buffer
  constexpr std::int64_t kChunkSize = 64;  // cubes are stored in chunks of 64

  // if true, need to reload everything in instances_vbo
  bool flag_full_sub_data = reinitialize_instances;

  if (cuboids_.size() > instances_vbo_capacity_) {
    flag_full_sub_data = true;

    instances_vbo_capacity_ = (cuboids_.size() + kChunkSize - 1) / kChunkSize * kChunkSize;
    instances_vbo_.SetData(instances_vbo_capacity_*sizeof(CuboidInstance),
        nullptr, kInstanceVboUsage);
  }
  if (instances_vbo_capacity_ - (std::int64_t)cuboids_.size() > kChunkSize) {
    flag_full_sub_data = true;

    instances_vbo_capacity_ = (cuboids_.size() + kChunkSize - 1) / kChunkSize * kChunkSize;
    instances_vbo_.SetData(instances_vbo_capacity_*sizeof(CuboidInstance),
        nullptr, kInstanceVboUsage);
  }
  
  std::size_t instances_cnt = cuboids_.size();

  // important: should be instances_vbo_capacity_
  // otherwise segfault (pointer out of bounds)
  instances_.reserve(instances_vbo_capacity_);

  std::int64_t prev_size = instances_.size();
  if (reinitialize_instances) {
    instances_.clear();

    for (auto& [cuboid_ptr, val]: cuboids_) {
      auto& cuboid = *cuboid_ptr;
      auto& [cuboid_unique_ptr_, index] = val;

      glm::mat4 model(1.0f);
      glm::vec3 pos(cuboid.pos);
      model = glm::translate(model, pos);

      index = instances_.size();
      instances_.push_back(CuboidInstance{model, cuboid.diffuse_color,
          cuboid.specular_color, cuboid.shininess});
    }
  } else {
    for (auto& cuboid_ptr : updated_cuboids) {
      auto& cuboid = *cuboid_ptr;

      glm::mat4 model(1.0f);
      glm::vec3 pos(cuboid.pos);
      model = glm::translate(model, pos);
      if (auto& index = cuboids_[cuboid_ptr].second; index == -1) {
        index = instances_.size();
        instances_.push_back(CuboidInstance{model, cuboid.diffuse_color,
            cuboid.specular_color, cuboid.shininess});
      } else {
        instances_[index] = CuboidInstance{model, cuboid.diffuse_color,
            cuboid.specular_color, cuboid.shininess};
      }
    }
  }
  updated_cuboids.clear();

  if (flag_full_sub_data) {
    instances_vbo_.SubData(0, instances_cnt * sizeof(CuboidInstance),
        (const std::byte*)instances_.data());
  } else {
    std::int64_t updated_size = (instances_.size() - prev_size) * sizeof(CuboidInstance);
    const std::byte* ptr = reinterpret_cast<const std::byte*>(instances_.data());
    ptr += updated_size;
    instances_vbo_.SubData(prev_size*sizeof(CuboidInstance), updated_size, ptr);
  }

  vao_.Bind();
  glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0, static_cast<GLsizei>(instances_cnt));
  vao_.Unbind();
}

void CuboidRenderer::NotifyUpdated(void* cuboid_void) {
  Cuboid* cuboid_ptr = reinterpret_cast<Cuboid*>(cuboid_void);
  updated_cuboids.push_back(cuboid_ptr);
}

void CuboidRenderer::Destroy(void* cuboid_void) {
  Cuboid* cuboid_ptr = reinterpret_cast<Cuboid*>(cuboid_void);
  cuboids_.erase(cuboid_ptr);
  reinitialize_instances = true;
}



}