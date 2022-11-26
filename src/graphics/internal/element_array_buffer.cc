#include <simple3d/graphics/internal/element_array_buffer.h>

#include <cstdint>
#include <cstddef>

#include <glad/gles2.h>

#include <simple3d/graphics/internal/misc.h>

namespace Simple3D::Internal {



constexpr auto kDefaultUsage = GL_STATIC_DRAW;

ElementArrayBuffer::ElementArrayBuffer(): size_{0} {
    glGenBuffers(1, &ebo_);
}

ElementArrayBuffer::ElementArrayBuffer(std::size_t size, std::uint32_t* data): size_{size} {
    glGenBuffers(1, &ebo_);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, static_cast<void*>(data), kDefaultUsage);
    Unbind();
}

ElementArrayBuffer::ElementArrayBuffer(const ElementArrayBuffer& other): ElementArrayBuffer{} {
    size_ = other.size_;
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_, nullptr, kDefaultUsage);
    Unbind();
    static_assert(false, "ctor isn't complete");
    // glBindBuffer()
}
//...

void ElementArrayBuffer::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
}

void ElementArrayBuffer::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, kGlesInvalidBuffer);
}



}