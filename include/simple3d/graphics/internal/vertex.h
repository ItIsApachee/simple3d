#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_H_

#include <glad/gles2.h>
#include <glm/glm.hpp>

namespace Simple3D::Internal {



struct Vertex {
 public:
  static void BindAttributes();

  glm::vec3 position{0.f, 0.f, 0.f};
  glm::vec3 normal{1.f, 0.f, 0.f};
};

struct TexturedVertex {
 public:
  // TODO(apachee): add more customization
  static void BindAttributes();

  glm::vec3 position{0.f, 0.f, 0.f};
  glm::vec3 normal{1.f, 0.f, 0.f};
  glm::vec2 texture_coords{0.f, 0.f};
};



}

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_H_
