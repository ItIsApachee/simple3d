#include "helpers.h"

#include "public.h"

#include "vao.h"
#include "shader_program.h"

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

void BindBuffer(EBufferType type, GLuint handle)
{
    glBindBuffer(static_cast<GLenum>(type), handle);
}

void UnbindBuffer(EBufferType type)
{
    // NB(apachee): It is a waste to do that in WebGL,
    // so skip unbinding in release.
#ifndef NDEBUG
    glBindBuffer(static_cast<GLenum>(type), InvalidBufferHandle);
#else
    S3D_UNUSED(type);
#endif
}

void CopyBuffer(
    GLuint srcBuffer,
    GLuint dstBuffer,
    GLintptr srcBufferOffset,
    GLintptr dstBufferOffset,
    GLsizeiptr size)
{
    BindBuffer(EBufferType::CopyReadBuffer, srcBuffer);
    BindBuffer(EBufferType::CopyWriteBuffer, dstBuffer);
    glCopyBufferSubData(
        static_cast<GLenum>(EBufferType::CopyReadBuffer),
        static_cast<GLenum>(EBufferType::CopyWriteBuffer),
        srcBufferOffset,
        dstBufferOffset,
        size);
    UnbindBuffer(EBufferType::CopyReadBuffer);
    UnbindBuffer(EBufferType::CopyWriteBuffer);
}

void ResetActiveBindings() {
    TShaderProgram::ActiveHandle_ = InvalidShaderHandle;
    TVao::ActiveHandle_ = InvalidBufferHandle;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NGLib
