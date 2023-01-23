#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_SPHERE_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_SPHERE_H_

#include <simple3d/graphics/internal/vertex.h>
#include <simple3d/graphics/models/primitive_renderer.h>

#include <cstdint>
#include <utility>
#include <vector>

namespace Simple3D {

namespace Internal {

std::pair<std::vector<Internal::Vertex>, std::vector<GLuint>> GenSphereGeometry(
    std::uint64_t detalization, bool normal_lerp);

}  // namespace Internal

template <std::uint64_t Detalization = 1, bool NormalLerp = true>
struct BaseSphere {
 public:
  using Renderer = PrimitiveRenderer<BaseSphere>;
  static const std::vector<Internal::Vertex>& GetVertices();
  static const std::vector<GLuint>& GetIndices();

  BaseSphere(const glm::vec3& pos = glm::vec3(0.0f),
             const glm::vec3& diffuse_color = glm::vec3(1.0f),
             const glm::vec3& specular_color = glm::vec3(1.0f),
             const GLfloat& shininess = 16.0f);
  BaseSphere(const BaseSphere&) = default;
  BaseSphere& operator=(const BaseSphere&) = default;
  ~BaseSphere() = default;

  explicit operator Internal::PrimitiveInstance() const;

  // coordinates
  glm::vec3 pos = glm::vec3(0.0f);

  // material
  glm::vec3 diffuse_color = glm::vec3(1.0f);
  glm::vec3 specular_color = glm::vec3(1.0f);
  GLfloat shininess{16.0f};

 private:
  using geometry_t =
      std::pair<std::vector<Internal::Vertex>, std::vector<GLuint>>;
  static const typename geometry_t& GetGeometry();
};

using Sphere = BaseSphere<2, true>;

// implementation
template <std::uint64_t Det, bool NormalLerp>
const typename BaseSphere<Det, NormalLerp>::geometry_t&
BaseSphere<Det, NormalLerp>::GetGeometry() {
  static const geometry_t& geometry =
      *new geometry_t{std::move(Internal::GenSphereGeometry(Det, NormalLerp))};
  return geometry;
}

template <std::uint64_t Det, bool NormalLerp>
const std::vector<Internal::Vertex>&
BaseSphere<Det, NormalLerp>::GetVertices() {
  return GetGeometry().first;
}

template <std::uint64_t Det, bool NormalLerp>
const std::vector<GLuint>& BaseSphere<Det, NormalLerp>::GetIndices() {
  return GetGeometry().second;
}

template <std::uint64_t Det, bool NormalLerp>
BaseSphere<Det, NormalLerp>::BaseSphere(const glm::vec3& pos,
                                        const glm::vec3& diffuse_color,
                                        const glm::vec3& specular_color,
                                        const GLfloat& shininess)
    : pos{pos},
      diffuse_color{diffuse_color},
      specular_color{specular_color},
      shininess{shininess} {}

template <std::uint64_t Det, bool NormalLerp>
BaseSphere<Det, NormalLerp>::operator Internal::PrimitiveInstance() const {
  glm::mat4 model(1.0f);
  glm::vec3 pos(pos);
  model = glm::translate(model, pos);

  return {model, diffuse_color, specular_color, shininess};
}

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_SPHERE_H_
