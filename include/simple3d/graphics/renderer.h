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

  virtual void Draw(IShader&) = 0;

  virtual void NotifyUpdated(void*) = 0;
  virtual void Destroy(void*) = 0;
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_RENDERER_H_
