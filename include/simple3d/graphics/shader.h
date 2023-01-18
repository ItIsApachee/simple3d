#ifndef INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_

#include <unordered_set>
#include <memory>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <simple3d/types.h>
#include <simple3d/graphics/light.h>

namespace Simple3D {



// support at least 8 lights
class IShader {
 public:
  IShader() = default;
  virtual ~IShader() = default;

  virtual void SetView(const glm::mat4&) = 0;
  virtual void SetProj(const glm::mat4&) = 0;
  virtual void SetViewPos(const glm::vec3&) = 0;

  virtual void SetDirectionalLights(const std::unordered_set<std::shared_ptr<DirectionalLight>>&) = 0;
  virtual void SetAmbientLight(const glm::vec3&) = 0;

  virtual void Use() = 0;
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_