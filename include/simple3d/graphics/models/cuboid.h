#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_

#include <vector>

#include <simple3d/types.h>
#include <simple3d/graphics/renderer.h>

namespace Simple3D {



struct Cuboid {
  using Renderer = CuboidRenderer;

  float x{0.};
  float y{0.};
  float z{0.};
};

class CuboidRenderer : public IRenderer {
 public:
  CuboidRenderer() = default;
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
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
