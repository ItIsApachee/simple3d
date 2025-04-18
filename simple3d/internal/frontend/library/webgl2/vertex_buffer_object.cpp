#include <GLES3/gl3.h>
#include <simple3d/context/context.h>
#include <simple3d/graphics/internal/vertex_buffer_object.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace Simple3D::Internal {

constexpr auto kVboTarget = GL_ARRAY_BUFFER;

// deprecated: no reason to use except default ctor
// VertexBufferObject::VertexBufferObject(bool generate,
//     GLenum usage) : usage_{usage}, size_{0} {
//   if (generate)
//     glGenBuffers(1, &vbo_);
// }

VertexBufferObject::VertexBufferObject(std::size_t size, const std::byte* data,
                                       GLenum usage)
    : size_{size}, ctx_id_{App::GetCtxId()}, usage_{usage} {
  if (ctx_id_ != 0) {
    glGenBuffers(1, &vbo_);

    Bind();
    glBufferData(kVboTarget, size_, reinterpret_cast<const void*>(data),
                 usage_);
  }
}

VertexBufferObject::VertexBufferObject(VertexBufferObject&& other)
    : VertexBufferObject() {
  std::swap(vbo_, other.vbo_);
  std::swap(ctx_id_, other.ctx_id_);
  std::swap(size_, other.size_);
  std::swap(usage_, other.usage_);
}

VertexBufferObject& VertexBufferObject::operator=(VertexBufferObject&& other) {
  if (&other == this) return *this;
  if (vbo_ != kGlesInvalidBuffer && ctx_id_ == App::GetCtxId()) {
    glDeleteBuffers(1, &vbo_);
  }

  vbo_ = other.vbo_;
  other.vbo_ = kGlesInvalidBuffer;

  ctx_id_ = other.ctx_id_;
  other.ctx_id_ = 0;

  size_ = other.size_;
  other.size_ = 0;

  usage_ = other.usage_;
  other.usage_ = kDefaultUsage;

  return *this;
}

VertexBufferObject::~VertexBufferObject() {
  if (vbo_ != kGlesInvalidBuffer && ctx_id_ == App::GetCtxId())
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
  glBufferSubData(kVboTarget, offset, size,
                  reinterpret_cast<const void*>(data));
}

void VertexBufferObject::Bind() const {
  if (vbo_ != kGlesInvalidBuffer) BindBuffer(kVboTarget, vbo_);
}

// void VertexBufferObject::Unbind() {
//   UnbindBuffer(kVboTarget);
// }

GLuint VertexBufferObject::vbo() const { return vbo_; }

std::size_t VertexBufferObject::size() const { return size_; }

GLenum VertexBufferObject::usage() const { return usage_; }

}  // namespace Simple3D::Internal
