#ifndef INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace Simple3D {



class IShader {
 public:
  IShader() = default;
  virtual ~IShader() = default;

  virtual void SetView(const glm::mat4&) = 0;
  virtual void SetProj(const glm::mat4&) = 0;
  virtual void SetViewPos(const glm::vec3&) = 0;
  virtual void Use() = 0;

  // TODO(apahcee): add lighting support
};



}

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_