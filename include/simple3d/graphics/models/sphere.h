/** \~Russian
 * \file
 * \brief Примитив сфера.
 */
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_SPHERE_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_SPHERE_H_

#include <simple3d/graphics/internal/vertex.h>
#include <simple3d/graphics/models/primitive_renderer.h>

#include <cstdint>
#include <utility>
#include <vector>

namespace Simple3D {

namespace Internal {

/** \~Russian
 * \brief Функция для генерации геометрии сферы.
 */
std::pair<std::vector<Internal::Vertex>, std::vector<GLuint>> GenSphereGeometry(
    std::uint64_t detalization, bool normal_lerp);

}  // namespace Internal

/** \~Russian
 * \brief Максимальное значение параметра Detalization шаблонного класса
 * template <std::uint64_t Detalization, bool NormalLerp>
 * struct BaseSphere;
 *
 * Данное ограничение вызвано тем, что количество вершин
 * сферы растет экспоненциально от Detalization.
 */
constexpr std::uint64_t kMaxSphereDetalization = 8;

/** \~Russian
 * \struct BaseSphere
 * \brief Примитив сфера.
 * \tparam Detalization Уровень детализации сферы.
 * \tparam NormalLerp Параметр, условно включающий линейную интерполяцию
 * нормалей с последующей нормализацией для создания эффекта "гладкости"
 * сферы.
 *
 * Для приближения сферы используется икосаэдр, над которым
 * Detalization раз выполняют преобразование каждой грани (треугольника) в 4
 * новых так, чтобы вершины оставались на поверхности сферы.
 * Подробнее: https://en.wikipedia.org/wiki/Geodesic_polyhedron
 */
template <std::uint64_t Detalization, bool NormalLerp>
struct BaseSphere {
  static_assert(Detalization <= kMaxSphereDetalization,
                "Detalization exceeds kMaxSphereDetalization");

 public:
  /** \~Russian
   * \brief Рендерер, используемый при вызове Scene::Create<BaseSphere>(...);
   */
  using Renderer = PrimitiveRenderer<BaseSphere>;

  /** \~Russian
   * \brief Реализация требований на шаблонный параметр P типа
   * PrimitiveRenderer.
   */
  static const std::vector<Internal::Vertex>& GetVertices();

  /** \~Russian
   * \brief Реализация требований на шаблонный параметр P типа
   * PrimitiveRenderer.
   */
  static const std::vector<GLuint>& GetIndices();

  BaseSphere(const glm::vec3& pos = glm::vec3(0.0f),
             const glm::vec3& diffuse_color = glm::vec3(1.0f),
             const glm::vec3& specular_color = glm::vec3(1.0f),
             const GLfloat& shininess = 16.0f);
  BaseSphere(const BaseSphere&) = default;
  BaseSphere& operator=(const BaseSphere&) = default;
  ~BaseSphere() = default;

  /** \~Russian
   * \brief Реализация требований на шаблонный параметр P типа
   * PrimitiveRenderer.
   */
  explicit operator Internal::PrimitiveInstance() const;

  /** \~Russian
   * \brief Позиция сферы.
   */
  glm::vec3 pos = glm::vec3(0.0f);

  /** \~Russian
   * \brief Цвет используемый для расчета фонового и рассеянного освещения.
   */
  glm::vec3 diffuse_color = glm::vec3(1.0f);

  /** \~Russian
   * \brief Цвет используемый для расчета освещения глянцевых бликов.
   */
  glm::vec3 specular_color = glm::vec3(1.0f);

  /** \~Russian
   * \brief Блеск используемый для расчета освещения глянцевых бликов.
   */
  GLfloat shininess{16.0f};

 private:
  using geometry_t =
      std::pair<std::vector<Internal::Vertex>, std::vector<GLuint>>;
  static const geometry_t& GetGeometry();
};

/** \~Russian
 * \brief Псевдоним типа BaseSphere с параметрами,
 * совмещающими высокую производительность и
 * хорошее качество изображения.
 */
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
