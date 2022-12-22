#ifndef INCLUDE_SIMPLE3D_GRAPHICS_RENDERER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_RENDERER_H_

#include <unordered_map>

#include <glm/mat4x4.hpp>

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

  virtual void Draw(const glm::mat4 &view, const glm::mat4 &proj) = 0;
  virtual void Destroy(Scene* scene) = 0;
};

namespace Internal {
    // should it be a class?
    // adv.: can make it a private static field, and
    // then access is provided only using friends
    template <typename R>
    std::unordered_map<Scene*, R> RendererStorage{};
}  // namespace Internal
// TODO(apachee): add is_renderer trait



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_RENDERER_H_
