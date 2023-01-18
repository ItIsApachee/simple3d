#ifndef INCLUDE_SIMPLE3D_GRAPHICS_LIGHT_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_LIGHT_H_

#include <glad/gles2.h>

#include <glm/vec3.hpp>

namespace Simple3D {



struct DirectionalLight {
  glm::vec3 direction = glm::vec3(0.0f);
  glm::vec3 diffuse = glm::vec3(0.0f);
  glm::vec3 specular = glm::vec3(0.0f);
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_LIGHT_H_
