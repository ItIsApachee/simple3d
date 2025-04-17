/** \~Russian
 * \file
 * \brief POD-объекты для передачи данных в шейдеры.
 */
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_H_

#include <glad/gles2.h>

#include <glm/glm.hpp>

namespace Simple3D::Internal {

/** \~Russian
 * \struct Vertex
 * \brief Вершина.
 */
struct Vertex {
 public:
  // TODO(apachee): move to ModelShader
  /** \~Russian
   * \brief Привязка активного VertexBufferObject к активному
   * VertexArrayObject.
   *
   * Вспомогательный метод при использовании Simple3D::ModelShader.
   * Активный VertexBufferObject должен представлять собой массив
   * из Vertex.
   */
  static void BindAttributes();

  /** \~Russian
   * \brief Позиция вершины.
   */
  glm::vec3 position{0.f, 0.f, 0.f};

  /** \~Russian
   * \brief Нормаль к поверхности.
   */
  glm::vec3 normal{1.f, 0.f, 0.f};
};

/** \~Russian
 * \struct TexturedVertex
 * \brief Текстурированная вершина.
 *
 * Аналог Vertex для поверхностей, использующих текстуры.
 */
struct TexturedVertex {
 public:
  // TODO(apachee): add more customization
  // TODO(apachee): move to TexturedModelShader
  /** \~Russian
   * \brief Привязка активного VertexBufferObject к активному
   * VertexArrayObject.
   *
   * Вспомогательный метод при использовании Simple3D::TexturedModelShader.
   * Активный VertexBufferObject должен представлять собой массив
   * из TexturedVertex.
   */
  static void BindAttributes();

  /** \~Russian
   * \brief Позиция вершины.
   */
  glm::vec3 position{0.f, 0.f, 0.f};

  /** \~Russian
   * \brief Нормаль к поверхности.
   */
  glm::vec3 normal{1.f, 0.f, 0.f};

  /** \~Russian
   * \brief Текстурные координаты данной вершины.
   */
  glm::vec2 texture_coords{0.f, 0.f};
};

}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_H_
