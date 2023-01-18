#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_

#include <unordered_set>
#include <chrono>
#include <utility>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glad/gles2.h>

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
  glm::vec3 pos = glm::vec3(0.0f);

  // material
  glm::vec3 diffuse_color = glm::vec3(1.0f);
  glm::vec3 specular_color = glm::vec3(1.0f);
  GLfloat shininess{16.0f};

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

  template <typename... Args>
  Cuboid* Create(Args&&... args);

  void Draw() override;
  void Destroy(void*) override;

 private:
  std::unordered_set<Cuboid*> cuboids_{};
  Internal::ElementBufferObject ebo_{};
  Internal::VertexBufferObject verices_vbo_{};

  Internal::VertexBufferObject instances_vbo_{};
  std::size_t instances_vbo_capacity_{0};

  Internal::VertexArrayObject vao_{};

  // // TODO(apachee): remove
  decltype(std::chrono::high_resolution_clock::now()) start_time_{};
};



// implementation
template <typename... Args>
Cuboid* CuboidRenderer::Create(Args&&... args) {
  Cuboid* cuboid_ptr =  new Cuboid{
    std::forward<Args>(args)...
  };
  cuboids_.insert(cuboid_ptr);

  return cuboid_ptr;
}



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
