#include <simple3d/graphics/internal/element_array_buffer.h>

#include <utility>
#include <cstdint>
#include <cstddef>

#include <glad/gles2.h>

#include <simple3d/graphics/internal/misc.h>

namespace Simple3D::Internal {



constexpr auto kDefaultUsage = GL_STATIC_DRAW;
constexpr auto kEboTarget = GL_ELEMENT_ARRAY_BUFFER;

ElementArrayBuffer::ElementArrayBuffer() {
  glGenBuffers(1, &ebo_);
}

ElementArrayBuffer::ElementArrayBuffer(std::size_t size, std::uint32_t* data)
    : size_{size} {
  glGenBuffers(1, &ebo_);
  Bind();
  glBufferData(
    kEboTarget, size, static_cast<void*>(data), kDefaultUsage);
  Unbind();
}

ElementArrayBuffer::ElementArrayBuffer(const ElementArrayBuffer& other)
    : ElementArrayBuffer{} {
  size_ = other.size_;
  Bind();
  glBufferData(kEboTarget, size_, nullptr, kDefaultUsage);
  Unbind();

  constexpr auto kCopyReadBuf = GL_COPY_READ_BUFFER;
  constexpr auto kCopyWriteBuf = GL_COPY_WRITE_BUFFER;
  BindBuffer(kCopyReadBuf, other.ebo_);
  BindBuffer(kCopyWriteBuf, ebo_);
  glCopyBufferSubData(
    kCopyReadBuf, kCopyWriteBuf, 0, 0, other.size_);
  UnbindBuffer(kCopyReadBuf);
  UnbindBuffer(kCopyWriteBuf);
}

ElementArrayBuffer::ElementArrayBuffer(ElementArrayBuffer&& other)
    : ElementArrayBuffer{} {
  std::swap(ebo_, other.ebo_);
  std::swap(size_, other.size_);
}

ElementArrayBuffer& ElementArrayBuffer::operator=(const ElementArrayBuffer& other) {
  static_assert(false, "incomplete operator=(const EBO&)");
}

ElementArrayBuffer& ElementArrayBuffer::operator=(ElementArrayBuffer&& other) {
  
}

void ElementArrayBuffer::Bind() {
  BindBuffer(kEboTarget, ebo_);
}

void ElementArrayBuffer::Unbind() {
  UnbindBuffer(kEboTarget);
}



}  // namespace Simple3D::Internal
