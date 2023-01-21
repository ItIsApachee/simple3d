#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_

#include <glad/gles2.h>
#include <simple3d/graphics/internal/element_buffer_object.h>
#include <simple3d/graphics/internal/vertex_array_object.h>
#include <simple3d/graphics/internal/vertex_buffer_object.h>
#include <simple3d/graphics/models/model_shader.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/shader.h>
#include <simple3d/types.h>

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

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

struct CuboidInstance;

class CuboidRenderer : public IRenderer {
 public:
  using Shader = ModelShader;

  CuboidRenderer();
  CuboidRenderer(const CuboidRenderer&) = delete;
  CuboidRenderer(CuboidRenderer&&) = default;
  CuboidRenderer& operator=(const CuboidRenderer) = delete;
  CuboidRenderer& operator=(CuboidRenderer&&) = default;
  ~CuboidRenderer() override = default;

  template <typename... Args>
  Cuboid* Create(Args&&... args);

  void Draw(IShader&) override;

  void NotifyUpdated(void*) override;
  void Destroy(void*) override;

 private:
  bool reinitialize_instances{false};
  std::vector<Cuboid*> updated_cuboids{};

  std::unordered_map<Cuboid*, std::pair<std::unique_ptr<Cuboid>, std::int64_t>>
      cuboids_{};

  Internal::ElementBufferObject ebo_{};
  Internal::VertexBufferObject verices_vbo_{};

  Internal::VertexBufferObject instances_vbo_{};
  std::size_t instances_vbo_capacity_{0};

  std::vector<CuboidInstance> instances_{};

  Internal::VertexArrayObject vao_{};
};

// implementation
template <typename... Args>
Cuboid* CuboidRenderer::Create(Args&&... args) {
  // Cuboid* cuboid_ptr =  new Cuboid{std::forward<Args>(args)...};
  auto cuboid_ptr =
      std::unique_ptr<Cuboid>(new Cuboid{std::forward<Args>(args)...});
  auto cuboid_ptr_val = cuboid_ptr.get();
  cuboids_.emplace(cuboid_ptr_val, std::pair{std::move(cuboid_ptr), -1});
  updated_cuboids.push_back(cuboid_ptr_val);

  return cuboid_ptr_val;
}

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
