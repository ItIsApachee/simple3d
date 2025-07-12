#include "vbo.h"

#include "helpers.h"

#include <simple3d/core/assert.h>
#include <simple3d/core/error.h>

#include <utility>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

// FIXME(apachee): Batch glGenBuffers calls.
// Allocate like 1000 buffers and use them when needed.
// Allocate more if needed.

TVbo::TVbo(
    i64 size,
    const std::byte* data,
    GLenum usage)
    : Usage_{usage}
    , Size_{size}
{
    S3D_THROW_UNLESS(
        size >= 0,
        "Can't create VBO with negative size: {}",
        size);

    glGenBuffers(1, &Handle_);

    Bind();
    glBufferData(
        static_cast<GLenum>(EBufferType::ArrayBuffer),
        Size_,
        reinterpret_cast<const void*>(data),
        Usage_);
}

TVbo::TVbo(TVbo&& other)
    : TVbo()
{
    std::swap(Handle_, other.Handle_);
    std::swap(Size_, other.Size_);
    std::swap(Usage_, other.Usage_);
}

TVbo& TVbo::operator=(TVbo&& other)
{
    if (&other == this) return *this;

    // Clear state
    {
        TVbo vbo = std::move(*this);
    }

    // Swap with cleared state
    std::swap(Handle_, other.Handle_);
    std::swap(Size_, other.Size_);
    std::swap(Usage_, other.Usage_);

    return *this;
}

TVbo::~TVbo()
{
    if (Handle_ != InvalidBufferHandle) {
        glDeleteBuffers(1, &Handle_);
    }
}

void TVbo::SetData(
    i64 size,
	const std::byte* data,
	GLenum usage)
{
    S3D_THROW_UNLESS(size >= 0, "Buffer size can't be negative: {}", size);

    Size_ = size;
    Usage_ = usage;

    Bind();
    glBufferData(
        static_cast<GLenum>(EBufferType::ArrayBuffer),
        Size_,
        reinterpret_cast<const void*>(data),
        Usage_);
}

void TVbo::SubData(
    i64 offset,
	i64 size,
	const std::byte* data)
{
    S3D_THROW_UNLESS(offset >= 0, "Offset can't be negative: {}", offset);
    S3D_THROW_UNLESS(size >= 0, "Size can't be negative: {}", offset);

    if (size == 0) {
        return;
    }

    S3D_THROW_UNLESS(
        offset + size <= Size_,
        "VBO of size {} can't fit buffer of size {} at offset {}",
        Size_,
        size,
        offset);

    Bind();
    glBufferSubData(
        static_cast<GLenum>(EBufferType::ArrayBuffer),
		offset,
		size,
		reinterpret_cast<const void*>(data));
}

void TVbo::Bind() const
{
    if (Handle_ != InvalidBufferHandle) {
        BindBuffer(EBufferType::ArrayBuffer, Handle_);
    }
}

GLuint TVbo::GetHandle() const
{
    return Handle_;
}

i64 TVbo::GetBufferSize() const {
    return Size_;
}

GLenum TVbo::GetUsage() const {
    return Usage_;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
