/** \~Russian
 * \dir
 * \brief Реализация оберток над объектами OpenGL.
 * 
 * Все объекты в данной директории находятся в пространстве имен
 * Simple3D::Internal.
*/
/** \~Russian
 * \file
 * \brief Вспомогательные инструменты, используемые
 * в реализации оберток над объектами OpenGL.
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_MISC_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_MISC_H_

#include <cstdint>
#include <GLES3/gl3.h>

/** \~Russian
 * \namespace Simple3D::Internal
 * \brief Внутренние части библиотеки и вспомогательные инструменты.
*/
namespace Simple3D::Internal {



/** \~Russian
 * \brief Идентификатор невалидного буфера.
 * 
 * Данный идентификатор взят из спецификации OpenGL ES 3.1.
*/
constexpr GLuint kGlesInvalidBuffer = 0;

/** \~Russian
 * \brief Идентификатор невалидного шейдера.
 * 
 * Данный идентификатор взят из спецификации OpenGL ES 3.1.
*/
constexpr GLuint kGlesInvalidShader = 0;

// only used as default value for VAO field, and
// shouldn't be used for any other reason whatsoever
/** \~Russian
 * \brief Умолчательный идентификатор VAO.
 * 
 * Данный идентификатор взят из спецификации OpenGL ES 3.1.
*/
constexpr GLenum kGlesDefaultVao = 0;

/** \~Russian
 * \brief Умолчательный идентификатор применения буфера.
*/
constexpr GLenum kDefaultUsage = GL_STATIC_DRAW;

/** \~Russian
 * \brief Функция для привязывания идентификатора buffer к
 * типу буфера target.
 * \param[in] target Тип буфера.
 * \param[in] buffer Привязываемый буфер.
*/
void BindBuffer(GLuint target, GLuint buffer);

/** \~Russian
 * \brief Функция для отвязывания буфера, привязанного
 * к типу буфера target.
 * \param[in] target Тип буфера.
*/
void UnbindBuffer(GLuint target);

/** \~Russian
 * \brief Функция для копирования буфера
 * \param[in] from Идентификатор буфера источника.
 * \param[in] to Идентификатор буфера назначения.
 * \param[in] from_offset Смещение буфера источника.
 * \param[in] to_offset Смещение буфера назначения.
 * \param[in] size Размер копируемой области.
*/
void CopyBuffer(GLuint from, GLuint to,
    GLintptr from_offset, GLintptr to_offset,
    GLsizeiptr size);

/** \~Russian
 * \brief Отвязывание всех активных привязок.
 * 
 * Используется App::Destroy. Данная функция не предназначения для
 * использования пользователем.
*/
void ResetActiveBindings();



}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_MISC_H_
