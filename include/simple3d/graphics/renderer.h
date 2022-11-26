#ifndef SIMPLE3D_GRAPHICS_RENDERER_H
#define SIMPLE3D_GRAPHICS_RENDERER_H

#include <unordered_map>

#include <simple3d/types.h>
#include <simple3d/graphics/scene.h>

namespace Simple3D {



/**
 * \class IRenderer simple3d/graphics/renderer.h
 * \brief Interface for renderers.
*/
class IRenderer {
public:
    IRenderer() = default;
    virtual ~IRenderer() = default;

    virtual void Draw() = 0;
    virtual void Destroy(Scene* scene) = 0;
};

namespace Internal {
    // should it be a class?
    // adv.: can make it a private static field, and 
    // then access is provided only using friends
    template <typename R>
    std::unordered_map<Scene*, R> RendererStorage{};
}
// TODO: add is_renderer trait



}

#endif