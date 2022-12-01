#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_MISC_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_MISC_H_

#include <cstdint>
#include <glad/gles2.h>

namespace Simple3D::Internal {
  constexpr GLuint kGlesInvalidBuffer = 0;
  constexpr auto kDefaultUsage = GL_STATIC_DRAW;

  void BindBuffer(GLuint target, GLuint buffer);
  void UnbindBuffer(GLuint target);
  void CopyBuffer(GLuint from, GLuint to,
    GLintptr from_offset, GLintptr to_offset,
    GLsizeiptr size);
}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_MISC_H_
