#ifndef INCLUDE_SIMPLE3D_GRAPHICS_RENDERER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_RENDERER_H_

#include <unordered_map>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <simple3d/types.h>
#include <simple3d/graphics/scene.h>
#include <simple3d/graphics/shader.h>

namespace Simple3D {



/**
 * \class IRenderer simple3d/graphics/renderer.h
 * \brief Interface for renderers.
*/
class IRenderer {
 public:
  IRenderer() = default;
  virtual ~IRenderer() = default;

  // virtual void Draw(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& view_pos) = 0;
  virtual void Destroy(Scene* scene) = 0;
};

namespace Internal {



// // should it be a class?
// // adv.: can make it a private static field, and
// // then access is provided only using friends
// template <typename R>
// std::unordered_map<Scene*, R> RendererStorage{};
template <typename R>
class RendererStorage {
 public:
  static RendererStorage& GetInstance() {
    static RendererStorage rs{};
    return rs;
  }

  RendererStorage(const RendererStorage&) = delete;
  RendererStorage(RendererStorage&&) = delete;
  RendererStorage& operator=(const RendererStorage&) = delete;
  RendererStorage& operator=(RendererStorage&&) = delete;
  ~RendererStorage() = default;

  std::unordered_map<Scene*, R> storage_{};

 private:
  RendererStorage() = default;
};



}  // namespace Internal
// TODO(apachee): add is_renderer trait



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_RENDERER_H_
