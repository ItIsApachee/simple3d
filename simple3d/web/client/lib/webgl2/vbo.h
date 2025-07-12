#pragma once

#include "public.h"

#include <GLES3/gl3.h>

#include <simple3d/core/types.h>

#include <cstddef>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

class TVbo
{
public:
    TVbo() = default;

    explicit TVbo(
        i64 size,
		const std::byte* data = nullptr,
		GLenum usage = DefaultBufferUsage);

    TVbo(TVbo&&);
    TVbo& operator=(TVbo&&);

    TVbo(const TVbo&) = delete;
    TVbo& operator=(const TVbo&) = delete;

    ~TVbo();

    void SetData(
        i64 size,
		const std::byte* data = nullptr,
		GLenum usage = DefaultBufferUsage);

    void SubData(
        i64 offset,
		i64 size,
		const std::byte* data);

    void Bind() const;

    GLuint GetHandle() const;

    i64 GetBufferSize() const;

    GLenum GetUsage() const;

private:
    GLuint Handle_{InvalidBufferHandle};
    GLenum Usage_{DefaultBufferUsage};
    i64 Size_{0};
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
