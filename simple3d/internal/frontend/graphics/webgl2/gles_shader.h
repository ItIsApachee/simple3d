/** \~Russian
 * \file
 * \brief Класс Simple3D::Internal::GlesShader.
 */

#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_GLES_SHADER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_GLES_SHADER_H_

#include <glad/gles2.h>
#include <simple3d/graphics/internal/misc.h>
#include <simple3d/misc/error.h>
#include <simple3d/types.h>

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <optional>
#include <string>

namespace Simple3D::Internal {

// TODO(apachee): change name to GlesShader
// TODO(apachee): consider adding support for compute shader, or add
// ComputeShader class

/** \~Russian
 * \class GlesShader
 * \brief Обертка над шейдером-программой OpenGL ES 3.1.
*/
class GlesShader {
 public:
  friend GlesShaderBuilder;
  friend void ResetActiveBindings();

  /** \~Russian
   * \brief Создает пустой шейдер-программу, использование
   * которой по назначению может привести к UB.
   * 
   * Для создания валидной шейдер-программы следует использовать
   * GlesShaderBuilder.
  */
  GlesShader() = default;
  GlesShader(const GlesShader&) = delete;
  GlesShader(GlesShader&&);

  GlesShader& operator=(const GlesShader&) = delete;
  GlesShader& operator=(GlesShader&&);

  ~GlesShader();

  /** \~Russian
   * \brief Метод для активации шейдера.
  */
  void Use() const;

  /** \~Russian
   * \brief Получить OpenGL ES 3.1 идентификатор шейдера-программы.
   * \return OpenGL ES 3.1 идентификатор шейдера-программы.
  */
  unsigned int GetID() const;

  /** \~Russian
   * \brief Проверка валидности шейдера.
   * \return true, если шейдер-программа не пустая, иначе false.
  */
  bool IsValid() const;

  /** \~Russian
   * \brief Метод для удаления программы-шейдера.
   * 
   * После удаления шейдер-программа становится пустой.
  */
  void Delete();

  /** \~Russian
   * \brief Устанавливает значение uniform с идентификатором name равным matrix.
   * \param[in] name Идентификатор.
   * \param[in] matrix Новое значение uniform.
   * \return Ошибка с типом ErrorType::kUniformNotFound, в случае если нет
   * uniform с заданным идентификатором, иначе пустая ошибка
   * 
   * Изменение uniform, тип которых отличается от mat4 вызывает UB.
  */
  Error SetUniformMat4fv(const std::string& name,
                         const glm::mat4& matrix) const;

  /** \~Russian
   * \brief Устанавливает значение uniform с идентификатором name равным vec.
   * \param[in] name Идентификатор.
   * \param[in] vec Новое значение uniform.
   * \return Ошибка с типом ErrorType::kUniformNotFound, в случае если нет
   * uniform с заданным идентификатором, иначе пустая ошибка
   * 
   * Изменение uniform, тип которых отличается от vec3 вызывает UB.
  */
  Error SetUniform3fv(const std::string& name, const glm::vec3& vec) const;

  /** \~Russian
   * \brief Устанавливает значение uniform с идентификатором name равным val.
   * \param[in] name Идентификатор.
   * \param[in] val Новое значение uniform.
   * \return Ошибка с типом ErrorType::kUniformNotFound, в случае если нет
   * uniform с заданным идентификатором, иначе пустая ошибка
   * 
   * Изменение uniform, тип которых отличается от int вызывает UB.
  */
  Error SetUniform1i(const std::string& name, const GLint& val) const;

 private:
  static GLuint active_shader_id_;

  GlesShader(GLuint shader_id, std::int64_t ctx_id);

  GLuint shader_id_{kGlesInvalidShader};
  std::int64_t ctx_id_{0};
};

/** \~Russian
 * \class GlesShaderBuilder
 * \brief Класс-builder для шейдера-программы.
 * 
 * Пример использования:
 * \code{.cpp}
 * // vertex_src - исходный код vertex шейдера
 * // framgent_src - исходный код fragment шейдера
 * Simple3D::Error err{};
 * auto shader = Simple3D::Internal::GlesShaderBuilder()
 *               .VertexShaderSource(vertex_src)
 *               .FragmentShaderSource(fragment_src)
 *               .Build(&err);
 * \endcode
*/
class GlesShaderBuilder {
  // FIXME(apachee): remove GlesShaderBuilder (no reason for a builder)
 public:
  GlesShaderBuilder() = default;
  GlesShaderBuilder(const GlesShaderBuilder&) = default;
  GlesShaderBuilder(GlesShaderBuilder&&) = default;

  GlesShaderBuilder& operator=(const GlesShaderBuilder&) = default;
  GlesShaderBuilder& operator=(GlesShaderBuilder&&) = default;

  ~GlesShaderBuilder() = default;

  /** \~Russian
   * \brief Метод для компиляции программы-шейдера по заданным параметрам.
   * \return Шейдер-программа, полученная в результате компиляции.
   * 
   * В случае ошибки возвращается пустая шейдер программа.
  */
  GlesShader Build();

  /** \~Russian
   * \brief Метод для компиляции программы-шейдера по заданным параметрам.
   * \param[out] error Ошибка компиляции.
   * \return Шейдер-программа, полученная в результате компиляции.
   * 
   * В случае ошибки возвращается пустая шейдер программа, а сама ошибка возвращается
   * через error.
  */
  GlesShader Build(Error* error);

  /** \~Russian
   * \brief Метод для установки исходного кода vertex шейдера.
   * \param[in] src Исходный код vertex шейдера.
   * \return Возвращает *this для поддержки цепочки вызовов.
  */
  GlesShaderBuilder& VertexShaderSource(std::string&& src);

  /** \~Russian
   * \brief Метод для установки исходного кода fragment шейдера.
   * \param[in] src Исходный код fragment шейдера.
   * \return Возвращает *this для поддержки цепочки вызовов.
  */
  GlesShaderBuilder& FragmentShaderSource(std::string&& src);

 private:
  std::optional<std::string> vertex_shader_src_{};
  std::optional<std::string> fragment_shader_src_{};
};

}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_GLES_SHADER_H_
