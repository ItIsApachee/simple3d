/** \~Russian
 * \dir
 * \brief Части библиотеки, используемые всеми компонентами.
*/
/** \~Russian
 * \file
 * \brief Класс Simple3D::Error.
*/
#ifndef INCLUDE_SIMPLE3D_MISC_ERROR_H_
#define INCLUDE_SIMPLE3D_MISC_ERROR_H_

#include <string>

#include <simple3d/types.h>

namespace Simple3D {



/** \~Russian
 * \enum ErrorType
 * \brief Перечисление возможных типов ошибок.
*/
enum class ErrorType : int {
  /** \~Russian
   * \brief Статус использоваемый в случае отсутствия ошибки.
  */
  kOk = 0,

  /** \~Russian
   * \brief Ошибка инициализации приложения.
  */
  kInitFailed,

  /** \~Russian
   * \brief Попытка инициализировать контекст, который уже инициализировали.
  */
  kAlreadyInitialized,

  /** \~Russian
   * \brief Ошибка при создании окна.
  */
  kWindowCreationFailed,

  /** \~Russian
   * \brief Ошибка при компиляции шейдера.
  */
  kShaderCompilationFailed,

  /** \~Russian
   * \brief Ошибка в случае отсутствия искомой uniform переменной шейдера.
  */
  kUniformNotFound,

  /** \~Russian
   * \brief Ошибка в случае неинициализированного шейдера.
  */
  kUnitializedShader,

  /** \~Russian
   * \brief Неизвестная ошибка.
  */
  kUnknown
};

/** \~Russian
 * \class Error
 * \brief Тип ошибки, возвращаемой библиотекой.
*/
struct Error {
 public:
  /** \~Russian
   * \brief Factory-метод для создания ошибки со статусом Simple3D::ErrorType::Ok.
   * \return Ошибка, используемая в случае отсутствия ошибки.
  */
  static Error Ok() {
    return Error(ErrorType::kOk);
  }

  explicit Error(
    ErrorType type = ErrorType::kUnknown, std::string description = ""):
    type{type}, description{description} {}
  Error(const Error&) = default;
  Error(Error&&) = default;
  Error& operator=(const Error&) = default;
  Error& operator=(Error&&) = default;

  ~Error() = default;

  /** \~Russian
   * \brief Метод для проверки отсутствия ошибки.
   * \return true, если ошибки нет. Иначе false.
  */
  bool IsOk() const {
    return type == ErrorType::kOk;
  }

  /** \~Russian
   * \brief Тип ошибки.
  */
  ErrorType type{ErrorType::kUnknown};

  /** \~Russian
   * \brief Описание ошибки.
  */
  std::string description{};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_MISC_ERROR_H_
