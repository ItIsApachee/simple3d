/** \~Russian
 * \file
 * \brief Класс Simple3D::Internal::VertexArrayObject.
 */
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_ARRAY_OBJECT_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_ARRAY_OBJECT_H_

#include <GLES3/gl3.h>
#include <simple3d/context/context.h>
#include <simple3d/gl/web/misc.h>
#include <simple3d/types.h>

#include <cstdint>

namespace Simple3D::Internal {

/** \~Russian
 * \class VertexArrayObject.
 * \brief Обертка над масивом вершин OpenGL ES 3.1.
 */
class VertexArrayObject {
 public:
  friend class ElementBufferObjectBuilder;
  friend void ResetActiveBindings();

  /** \~Russian
   * \brief Создает масив вершин.
   *
   * Создание масива вершин при неинициализированном контексте вызывает UB.
   */
  VertexArrayObject();
  VertexArrayObject(VertexArrayObject&&);
  VertexArrayObject& operator=(VertexArrayObject&&);
  VertexArrayObject(const VertexArrayObject&) = delete;
  VertexArrayObject& operator=(const VertexArrayObject&) = delete;
  ~VertexArrayObject();

  // TODO(apachee):
  // [ ] a way to define an array of generic vertex attribute data

  /** \~Russian
   * \brief Метод для привязки к массиву вершин буфера индексов.
   * \param[in] ebo Привязываемый буфер индексов.
   */
  void BindEbo(const ElementBufferObject& ebo) const;

  /** \~Russian
   * \brief Метод для активации буфера индексов.
   */
  void Bind() const;

  /** \~Russian
   * \brief Метод для деактивации буфера индексов.
   */
  static void Unbind();

 private:
  // bind optimization:
  // since there's only one render thread
  // we can statically store active VAO
  // to reduce the amount of binding calls
  static GLuint active_vao_;

  GLuint vao_{kGlesDefaultVao};
  std::int64_t ctx_id_{0};
};

}  // namespace Simple3D::Internal

#endif  //  INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_ARRAY_OBJECT_H_
