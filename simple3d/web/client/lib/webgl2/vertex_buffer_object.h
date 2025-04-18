/** \~Russian
 * \file
 * \brief Класс Simple3D::Internal::VertexBufferObject.
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_BUFFER_OBJECT_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_BUFFER_OBJECT_H_

#include <GLES3/gl3.h>
#include <simple3d/graphics/web/misc.h>

#include <cstddef>
#include <cstdint>

namespace Simple3D::Internal {

/** \~Russian
 * \class VertexBufferObject
 * \brief Обертка над буфером вершин OpenGL ES 3.1.
*/
class VertexBufferObject {
 public:
  /** \~Russian
   * \brief Создает пустой буфер вершин.
   * 
   * Использование пустого буфера вершин может привести к UB.
  */
  VertexBufferObject() = default;

  /** \~Russian
   * \brief Создает буфер вершин по заданным данным.
   * \param[in] size Размер буфера вершин в байтах.
   * \param[in] data Указатель на данные буфера. Значение nullptr аллоцирует пустую
   * область памяти заданного размера.
   * \param[in] usage Тип использования буфера согласно спецификации OpenGL ES 3.1.

   * Создание буфера вершин при неинициализированном контексте вызывает UB.
  */
  explicit VertexBufferObject(std::size_t size, const std::byte* data = nullptr,
                              GLenum usage = kDefaultUsage);
  VertexBufferObject(VertexBufferObject&&);
  VertexBufferObject& operator=(VertexBufferObject&&);
  VertexBufferObject(const VertexBufferObject&) = delete;
  VertexBufferObject& operator=(const VertexBufferObject&) = delete;
  ~VertexBufferObject();

  /** \~Russian
   * \brief Метод для изменения хранимого буфера.
   * \param[in] size Размер буфера вершин в байтах.
   * \param[in] data Указатель на данные буфера. Значение nullptr аллоцирует пустую
   * область памяти заданного размера.
   * \param[in] usage Тип использования буфера согласно спецификации OpenGL ES 3.1.
   * 
   * Каждый вызов данного метода реаллоцирует память буфера вершин.
  */
  void SetData(std::size_t size, const std::byte* data = nullptr,
               GLenum usage = kDefaultUsage);

  /** \~Russian
   * \brief Метод для изменения части хранимого буфера.
   * \param[in] offset Смещение изменяемой части относительно начала буфера.
   * \param[in] size Размер изменяемой части в байтах.
   * \param[in] data Указатель на данные.
   * 
   * В случае успешного вызова, участок памяти [offset; offset+size) станет равным
   * первым size байтам, полученным по указателю data.
  */
  void SubData(std::size_t offset, std::size_t size, const std::byte* data);

  /** \~Russian
   * \brief Метод для активации буфера вершин.
  */
  void Bind() const;
  // static void Unbind();

  /** \~Russian
   * \brief Метод для получения OpenGL ES 3.1 идентификатора буфера вершин.
  */
  GLuint vbo() const;

  /** \~Russian
   * \brief Метод для получения размера буфера в байтах.
  */
  std::size_t size() const;

  /** \~Russian
   * \brief Метод для получения типа хранимого буфера согласно спецификации
   * OpenGL ES 3.1.
  */
  GLenum usage() const;

 private:
  GLuint vbo_{kGlesInvalidBuffer};
  std::int64_t ctx_id_{0};
  GLenum usage_{kDefaultUsage};
  std::size_t size_{0};
};

}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_BUFFER_OBJECT_H_
