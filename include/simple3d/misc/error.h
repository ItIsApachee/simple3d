/** \file
 * \brief Definition of Simple3D::Error, Simple3D::ErrorType
*/
#ifndef INCLUDE_SIMPLE3D_MISC_ERROR_H_
#define INCLUDE_SIMPLE3D_MISC_ERROR_H_

#include <string>

#include <simple3d/types.h>

namespace Simple3D {



/** \enum ErrorType simple3d/misc/error.h
 * \brief Types of possibles errors.
*/
enum class ErrorType : int {
  kOk = 0,
  kInitFailed,
  kWindowCreationFailed,
  kShaderCompilationFailed,
  kUniformNotFound,
  kUnknown
};

/** \struct Error simple3d/misc/error.h
 * \brief POD Error for error handling.
*/
struct Error {
 public:
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

  bool IsOk() const {
    return type == ErrorType::kOk;
  }

  ErrorType type{ErrorType::kUnknown};
  std::string description{};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_MISC_ERROR_H_
