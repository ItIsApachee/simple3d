#include <simple3d/graphics/internal/misc.h>
#include <simple3d/graphics/internal/gles_shader.h>
#include <simple3d/graphics/internal/vertex_array_object.h>

#include <GLES3/gl3.h>

namespace Simple3D::Internal {




void BindBuffer(GLenum target, GLuint buffer) {
  glBindBuffer(target, buffer);
}

void UnbindBuffer(GLenum target) {
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

void ResetActiveBindings() {
  GlesShader::active_shader_id_ = kGlesInvalidShader;
  VertexArrayObject::active_vao_ = kGlesInvalidBuffer;
}



}  // namespace Simple3D::Internal
