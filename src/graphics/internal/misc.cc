#include <simple3d/graphics/internal/misc.h>

#include <glad/gles2.h>

namespace Simple3D::Internal {




void BindBuffer(GLuint target, GLuint buffer) {
  glBindBuffer(target, buffer);
}

void UnbindBuffer(GLuint target) {
  glBindBuffer(target, kGlesInvalidBuffer);
}

void CopyBuffer(GLuint from, GLuint to,
    GLintptr from_offset, GLintptr to_offset,
    GLsizeiptr size) {

  constexpr auto kCopyReadBuf = GL_COPY_READ_BUFFER;
  constexpr auto kCopyWriteBuf = GL_COPY_WRITE_BUFFER;
  BindBuffer(kCopyReadBuf, from);
  BindBuffer(kCopyWriteBuf, to);
  glCopyBufferSubData(
    kCopyReadBuf, kCopyWriteBuf, from_offset, to_offset, size);
  UnbindBuffer(kCopyReadBuf);
  UnbindBuffer(kCopyWriteBuf);
}



}  // namespace Simple3D::Internal
