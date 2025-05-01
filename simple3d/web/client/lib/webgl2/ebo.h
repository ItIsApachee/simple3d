#pragma once

#include "public.h"

#include <simple3d/core/types.h>

#include <GLES3/gl3.h>

#include <cstddef>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

class TEboBuilder
{
public:
    TEboBuilder() = default;

    TEboBuilder Data(
        i64 size,
        const std::byte *data = nullptr) &&;

    TEboBuilder Usage(GLenum usage) &&;

    TEbo Build(const TVao &vao) &&;

private:
    const std::byte *Data_{nullptr};
    i64 Size_{0};
    GLenum Usage_{kDefaultUsage};
};

////////////////////////////////////////////////////////////////////////////////

class TEbo
{
public:
    friend TEboBuilder;

    TEbo() = default;

    TEbo(TEbo &&);
    TEbo &operator=(TEbo &&);

    TEbo(const TEbo &) = delete;
    TEbo &operator=(const TEbo &) = delete;

    ~TEbo();

    GLuint GetHandle() const;

    GLenum GetUsage() const;

private:
    TEbo(
        GLuint ebo,
        i64 size,
        GLenum usage);

    GLuint Handle_ = InvalidBufferHandle;
    GLenum Usage_ = kDefaultUsage;
    i64 Size_ = 0;
};

////////////////////////////////////////////////////////////////////////////////

} // namespace NSimple3D::NWebGL2
