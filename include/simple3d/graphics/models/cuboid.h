#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_

#include <vector>
#include <chrono>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <simple3d/types.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/internal/element_buffer_object.h>
#include <simple3d/graphics/internal/vertex_buffer_object.h>
#include <simple3d/graphics/internal/vertex_array_object.h>
#include <simple3d/graphics/models/model_shader.h>

namespace Simple3D {



struct Cuboid {
  // TODO(apachee): add ability to change face's color

  using Renderer = CuboidRenderer;

  // coordinates
  float x{0.};
  float y{0.};
  float z{0.};

  // colors
  float r{0.};
  float g{0.};
  float b{0.};

  // TODO(apachee): add rotation
  // // rotation
  // float roll{0.};
  // float pitch{0.};
  // float yaw{0.};
};

class CuboidRenderer : public IRenderer {
 public:
  using Shader = ModelShader;

  CuboidRenderer();
  CuboidRenderer(const CuboidRenderer&) = delete;
  CuboidRenderer(CuboidRenderer&&) = default;
  CuboidRenderer& operator=(const CuboidRenderer) = delete;
  CuboidRenderer& operator=(CuboidRenderer&&) = default;
  ~CuboidRenderer() override;

  Cuboid* Create(float x, float y, float z);

  void Draw() override;

 private:
  std::vector<Cuboid*> cuboids_{};
  Internal::ElementBufferObject ebo_{};
  Internal::VertexBufferObject verices_vbo_{};

  Internal::VertexBufferObject instances_vbo_{};
  std::size_t instances_vbo_capacity_{0};

  Internal::VertexArrayObject vao_{};

  // // TODO(apachee): remove
  decltype(std::chrono::high_resolution_clock::now()) start_time_{};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
