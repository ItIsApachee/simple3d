/** \~Russian
 * \file
 * \brief Класс Simple3D::Internal::ElementBufferObject.
 */
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_BUFFER_OBJECT_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_BUFFER_OBJECT_H_

#include <glad/gles2.h>
#include <simple3d/graphics/internal/misc.h>
#include <simple3d/types.h>

#include <cstddef>
#include <cstdint>

namespace Simple3D::Internal {

/** \~Russian
 * \class ElementBufferObjectBuilder
 * \brief Класс-builder для ElementBufferObject.
 *
 * Пример использования:
 * \code{.cpp}
 * // indices_size_in_bytes - размер массива индексов в байт
 * // indices_data - указатель на данные массива индексов
 * Simple3D::Internal::VertexArrayObject vao{};
 * auto ebo = Simple3D::Internal::ElementBufferObjectBuilder()
 *                .Data(sizeof(indices_size_in_bytes,
 *                      indices_data)
 *                .Build(vao);
 * \endcode
 *
 */
class ElementBufferObjectBuilder {
 public:
  ElementBufferObjectBuilder() = default;
  ElementBufferObjectBuilder(const ElementBufferObjectBuilder&) = default;
  ElementBufferObjectBuilder(ElementBufferObjectBuilder&&) = default;
  ElementBufferObjectBuilder& operator=(const ElementBufferObjectBuilder&) =
      default;
  ElementBufferObjectBuilder& operator=(ElementBufferObjectBuilder&&) = default;
  ~ElementBufferObjectBuilder() = default;

  /** \~Russian
   * \brief Метод для установки данных массива индексов.
   * \param[in] size Размер массива индексов в байт.
   * \param[in] data Указатель на данные массива индексов.
   * \return Возвращает *this для поддержки цепочки вызовов.
   */
  ElementBufferObjectBuilder& Data(std::size_t size,
                                   const std::byte* data = nullptr);

  /** \~Russian
   * \brief Метод для установки типа использования буфера.
   * \param[in] usage Тип использования буфера согласно спецификации OpenGL ES
   * 3.1.
   * \return Возвращает *this для поддержки цепочки вызовов.
   */
  ElementBufferObjectBuilder& Usage(GLenum usage);

  /** \~Russian
   * \brief Метод для создания буфера с использованием vao.
   * \param[in] vao Vertex array object используемый для создания.
   * \return Возращает ElementBufferObject по параметрам заданным ранее.
   */
  ElementBufferObject Build(const VertexArrayObject& vao);

 private:
  const std::byte* data_{nullptr};
  std::size_t size_{0};
  GLenum usage_{kDefaultUsage};
};

/** \~Russian
 * \class ElementBufferObject
 * \brief Обертка над буфером индексов.
 */
class ElementBufferObject {
 public:
  friend ElementBufferObjectBuilder;

  /** \~Russian
   * \brief Создает пустой буфер индексов, использование которого может
   * привести к UB.
   *
   * Для создания валидного буфера индексов следует использовать
   * ElementBufferObjectBuilder.
   */
  ElementBufferObject() = default;
  ElementBufferObject(ElementBufferObject&&);
  ElementBufferObject& operator=(ElementBufferObject&&);
  ElementBufferObject(const ElementBufferObject&) = delete;
  ElementBufferObject& operator=(const ElementBufferObject&) = delete;
  ~ElementBufferObject();

  /** \~Russian
   * \brief Метод для получения идентификатора буфера индексов.
   */
  GLuint ebo() const;

  /** \~Russian
   * \brief Метод для получения идентификатора типа использования буфера
   * согласно спецификации OpenGL ES 3.1.
   */
  GLenum usage() const;

 private:
  ElementBufferObject(GLuint ebo, std::int64_t ctx_id, std::size_t size,
                      GLenum usage);

  GLuint ebo_{kGlesInvalidBuffer};  // handle for OpenGL buffer
  GLenum usage_{kDefaultUsage};     // usage of the buffer
  std::size_t size_{0};             // capacity of OpenGL buffer
  std::int64_t ctx_id_{0};
};

}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_BUFFER_OBJECT_H_
