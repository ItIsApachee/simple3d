#include "ebo.h"

#include "helpers.h"
#include "vao.h"

#include <simple3d/core/assert.h>
#include <simple3d/core/error.h>

#include <utility>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

TEboBuilder TEboBuilder::Data(
    i64 size,
    const std::byte* data) &&
{
    S3D_THROW_UNLESS(
        size >= 0,
        "Can't create EBO with negative size: {}",
        size);

    Size_ = size;
    Data_ = data;
    return *this;
}

TEboBuilder TEboBuilder::Usage(GLenum usage) &&
{
    Usage_ = usage;
    return *this;
}

TEbo TEboBuilder::Build(
    const TVao& vao) &&
{
    GLuint handle{InvalidBufferHandle};
    glGenBuffers(1, &handle);

    vao.Bind();
    BindBuffer(EBufferType::ElementArrayBuffer, handle);

    glBufferData(static_cast<GLenum>(EBufferType::ElementArrayBuffer), Size_, static_cast<const void*>(Data_), Usage_);

    return TEbo(handle, Size_, Usage_);
}

////////////////////////////////////////////////////////////////////////////////

TEbo::TEbo(
    GLuint handle,
    i64 size,
    GLenum usage)
    : Handle_{handle}
    , Usage_{usage}
    , Size_{size}
{
    S3D_THROW_UNLESS(
        size >= 0,
        "Can't create EBO with negative size: {}",
        size);
}

TEbo::TEbo(
    TEbo&& other)
    : TEbo()
{
    std::swap(Handle_, other.Handle_);
    std::swap(Size_, other.Size_);
    std::swap(Usage_, other.Usage_);
}

TEbo& TEbo::operator=(
    TEbo&& other)
{
    if (&other == this) return *this;

    // Clear state
    {
        TEbo TEbo = std::move(*this);
    }

    // Swap with cleared state
    std::swap(Handle_, other.Handle_);
    std::swap(Size_, other.Size_);
    std::swap(Usage_, other.Usage_);

    return *this;
}

TEbo::~TEbo()
{
    if (Handle_ != InvalidBufferHandle) {
        glDeleteBuffers(1, &Handle_);
    }
}

GLuint TEbo::GetHandle() const
{
    return Handle_;
}

GLenum TEbo::GetUsage() const
{
    return Usage_;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace NSimple3D::NWebGL2
