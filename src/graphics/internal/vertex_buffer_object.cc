#include <simple3d/graphics/internal/vertex_buffer_object.h>

#include <cstddef>
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

VertexBufferObject::VertexBufferObject(std::size_t size, std::byte* data,
    GLenum usage) : size_{size}, usage_{usage} {
  glGenBuffers(1, &vbo_);

  Bind();
  glBufferData(kVboTarget, size_, reinterpret_cast<void*>(data), usage_);
  Unbind();
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
}

VertexBufferObject::~VertexBufferObject() {
  if (vbo_ != kGlesInvalidBuffer)
    glDeleteBuffers(1, &vbo_);
}

void VertexBufferObject::Bind() {
  if (vbo_ != kGlesInvalidBuffer)
    BindBuffer(kVboTarget, vbo_);
}

void VertexBufferObject::Unbind() {
  UnbindBuffer(kVboTarget);
}

GLuint VertexBufferObject::vbo() {
  return vbo_;
}

GLenum VertexBufferObject::usage() {
  return usage_;
}



}  // namespace Simple3D::Internal
