#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_

#include <vector>
#include <chrono>

#include <simple3d/types.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/internal/element_buffer_object.h>
#include <simple3d/graphics/internal/vertex_buffer_object.h>
#include <simple3d/graphics/internal/vertex_array_object.h>

namespace Simple3D {



struct Cuboid {
  using Renderer = CuboidRenderer;

  float x{0.};
  float y{0.};
  float z{0.};
};

class CuboidRenderer : public IRenderer {
 public:
  CuboidRenderer();
  CuboidRenderer(const CuboidRenderer&) = delete;
  CuboidRenderer(CuboidRenderer&&) = default;
  CuboidRenderer& operator=(const CuboidRenderer) = delete;
  CuboidRenderer& operator=(CuboidRenderer&&) = default;
  ~CuboidRenderer();

  Cuboid* Create(float x, float y, float z);

  void Draw() override;
  void Destroy(Scene* scene) override;

 private:
  std::vector<Cuboid*> cuboids_{};
  Internal::ElementBufferObject ebo_{};
  Internal::VertexBufferObject vbo_{};
  // Internal::VertexBufferObject position_vbo_{};
  Internal::VertexArrayObject vao_{};

  decltype(std::chrono::high_resolution_clock::now()) start_time_{};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
