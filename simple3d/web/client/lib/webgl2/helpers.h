#pragma once

#include <cstdint>

#include <GLES3/gl3.h>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

enum struct EBufferType : GLenum {
    ArrayBuffer = GL_ARRAY_BUFFER,
    CopyReadBuffer = GL_COPY_READ_BUFFER,
    CopyWriteBuffer = GL_COPY_WRITE_BUFFER,
    ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
    PixelPackBuffer = GL_PIXEL_PACK_BUFFER,
    PixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
    TransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER,
    UniformBuffer = GL_UNIFORM_BUFFER,
};

////////////////////////////////////////////////////////////////////////////////

void BindBuffer(EBufferType type, GLuint handle);

void UnbindBuffer(EBufferType type);

void CopyBuffer(
    GLuint srcBuffer,
    GLuint dstBuffer,
    GLintptr srcBufferOffset,
    GLintptr dstBufferOffset,
    GLsizeiptr size);

void ResetActiveBindings();

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
