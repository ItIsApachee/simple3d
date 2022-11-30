#include <simple3d/graphics/internal/element_array_buffer.h>

#include <utility>
#include <cstdint>
#include <cstddef>

#include <glad/gles2.h>

#include <simple3d/graphics/internal/misc.h>

namespace Simple3D::Internal {



constexpr auto kEboTarget = GL_ELEMENT_ARRAY_BUFFER;

ElementBufferObject::ElementBufferObject(bool generate, GLenum usage)
    : size_{0}, usage_{usage} {
  if (generate)
    glGenBuffers(1, &ebo_);
}

ElementBufferObject::ElementBufferObject(
    std::size_t size, std::uint32_t* data, GLenum usage)
    : size_{size}, usage_{usage} {
  glGenBuffers(1, &ebo_);
  Bind();
  glBufferData(
    kEboTarget, size, static_cast<void*>(data), usage_);
  Unbind();
}

ElementBufferObject::ElementBufferObject(const ElementBufferObject& other)
    : ElementBufferObject{} {
  size_ = other.size_;
  Bind();
  glBufferData(kEboTarget, size_, nullptr, other.usage_);
  Unbind();

  CopyBuffer(other.ebo_, ebo_, 0, 0, size_);
}

ElementBufferObject::ElementBufferObject(ElementBufferObject&& other)
    : ElementBufferObject{} {
  std::swap(ebo_, other.ebo_);
  std::swap(size_, other.size_);
}

ElementBufferObject& ElementBufferObject::operator=(const ElementBufferObject& other) {
  if (&other == this)
    return *this;
  if (size_ != other.size_ || usage_ != other.usage_) {
    Bind();
    glBufferData(
      kEboTarget, other.size_, nullptr, other.usage_);
    Unbind();
    size_ = other.size_;
    usage_ = other.usage_;
  }
  
  CopyBuffer(other.ebo_, ebo_, 0, 0, size_);
  return *this;
}

ElementBufferObject& ElementBufferObject::operator=(ElementBufferObject&& other) {
  if (ebo_ != kGlesInvalidBuffer) {
    glDeleteBuffers(1, &ebo_);
  }

  ebo_ = other.ebo_;
  other.ebo_ = kGlesInvalidBuffer;

  size_ = other.size_;
  other.size_ = 0;

  return *this;
}

ElementBufferObject::~ElementBufferObject() {
  if (ebo_ != kGlesInvalidBuffer) {
    glDeleteBuffers(1, &ebo_);
  }
}

void ElementBufferObject::Bind() {
  BindBuffer(kEboTarget, ebo_);
}

void ElementBufferObject::Unbind() {
  UnbindBuffer(kEboTarget);
}

GLuint ElementBufferObject::ebo() {
  return ebo_;
}



}  // namespace Simple3D::Internal
