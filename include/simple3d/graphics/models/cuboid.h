#ifndef SIMPLE3D_GRAPHICS_MODELS_CUBOID_H
#define SIMPLE3D_GRAPHICS_MODELS_CUBOID_H

#include <vector>

#include <simple3d/types.h>
#include <simple3d/graphics/renderer.h>

namespace Simple3D {



struct Cuboid {
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



}

#endif