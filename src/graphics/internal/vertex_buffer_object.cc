#include <simple3d/graphics/internal/vertex_buffer_object.h>

#include <cstddef>
#include <cassert>
#include <utility>

#include <glad/gles2.h>

namespace Simple3D::Internal {



constexpr auto kVboTarget = GL_ARRAY_BUFFER;

// deprecated: no reason to use except default ctor
// VertexBufferObject::VertexBufferObject(bool generate,
//     GLenum usage) : usage_{usage}, size_{0} {
//   if (generate)
//     glGenBuffers(1, &vbo_);
// }

VertexBufferObject::VertexBufferObject(std::size_t size, const std::byte* data,
    GLenum usage) : size_{size}, usage_{usage} {
  glGenBuffers(1, &vbo_);

  Bind();
  glBufferData(kVboTarget, size_, reinterpret_cast<const void*>(data), usage_);
  // Unbind();
}

// deprecated: few reasons to use, and user can do it themself
// VertexBufferObject::VertexBufferObject(const VertexBufferObject& other)
//     : size_{other.size_}, usage_{other.usage_} {
//   if (other.vbo_ == kGlesInvalidBuffer) {
//     // constructing from empty buffer creates empty buffer
//     vbo_ = kGlesInvalidBuffer;
//     return;
//   }

//   // other is NOT an empty buffer
//   glGenBuffers(1, &vbo_);

//   Bind();
//   glBufferData(kVboTarget, size_, nullptr, usage_);
//   Unbind();

//   CopyBuffer(other.vbo_, vbo_, 0, 0, size_);
// }

VertexBufferObject::VertexBufferObject(VertexBufferObject&& other)
    : VertexBufferObject() {
  std::swap(vbo_, other.vbo_);
  std::swap(size_, other.size_);
  std::swap(usage_, other.usage_);
}

// deprecated: few reasons to use, and user can do it themself
// VertexBufferObject& VertexBufferObject::operator=(
//     const VertexBufferObject& other) {
//   if (&other == this)
//     return *this;
//   if (other.vbo_ == kGlesInvalidBuffer) {
//     // copying empty buffer creates empty buffer
//     // and destroys previous buffer if `this` is not an empty buffer
//     if (vbo_ != kGlesInvalidBuffer) {
//       glDeleteBuffers(1, &vbo_);
//       vbo_ = kGlesInvalidBuffer;
//     }
//     size_ = other.size_;
//     usage_ = other.usage_;
//     return *this;
//   }

//   // other is NOT an empty buffer

//   // reuse existing buffer instead of deleting & generating
//   if (vbo_ == kGlesInvalidBuffer) {
//     glGenBuffers(1, &vbo_);
//   }
//   if (size_ != other.size_ || usage_ != other.usage_) {
//     Bind();
//     glBufferData(kVboTarget, other.size_, nullptr, other.usage_);
//     Unbind();
//     size_ = other.size_;
//     usage_ = other.usage_;
//   }
//   CopyBuffer(other.vbo_, vbo_, 0, 0, size_);

//   return *this;
// }

VertexBufferObject& VertexBufferObject::operator=(VertexBufferObject&& other) {
  if (&other == this)
    return *this;
  if (vbo_ != kGlesInvalidBuffer) {
    glDeleteBuffers(1, &vbo_);
  }

  vbo_ = other.vbo_;
  other.vbo_ = kGlesInvalidBuffer;

  size_ = other.size_;
  other.size_ = 0;

  usage_ = other.usage_;
  other.usage_ = kDefaultUsage;

  return *this;
}

VertexBufferObject::~VertexBufferObject() {
  if (vbo_ != kGlesInvalidBuffer)
    glDeleteBuffers(1, &vbo_);
}

void VertexBufferObject::SetData(std::size_t size, const std::byte* data,
    GLenum usage) {
  size_ = size;
  usage_ = usage;

  Bind();
  glBufferData(kVboTarget, size_, reinterpret_cast<const void*>(data), usage_);
}

void VertexBufferObject::SubData(std::size_t offset, std::size_t size,
    const std::byte* data) {
  assert(offset + size <= size_);
  Bind();
  glBufferSubData(kVboTarget, offset, size, reinterpret_cast<const void*>(data));
}

void VertexBufferObject::Bind() const {
  if (vbo_ != kGlesInvalidBuffer)
    BindBuffer(kVboTarget, vbo_);
}

// void VertexBufferObject::Unbind() {
//   UnbindBuffer(kVboTarget);
// }

GLuint VertexBufferObject::vbo() const {
  return vbo_;
}

std::size_t VertexBufferObject::size() const {
  return size_;
}

GLenum VertexBufferObject::usage() const {
  return usage_;
}



}  // namespace Simple3D::Internal
