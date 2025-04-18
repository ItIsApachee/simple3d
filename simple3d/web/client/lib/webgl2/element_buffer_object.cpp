#include <GLES3/gl3.h>
#include <simple3d/context/context.h>
#include <simple3d/gl/web/element_buffer_object.h>
#include <simple3d/gl/web/misc.h>
#include <simple3d/gl/web/vertex_array_object.h>

#include <cstddef>
#include <cstdint>
#include <utility>

namespace Simple3D::Internal {

constexpr auto kEboTarget = GL_ELEMENT_ARRAY_BUFFER;

ElementBufferObjectBuilder& ElementBufferObjectBuilder::Data(
    std::size_t size, const std::byte* data) {
  size_ = size;
  data_ = data;
  return *this;
}

ElementBufferObjectBuilder& ElementBufferObjectBuilder::Usage(GLenum usage) {
  usage_ = usage;
  return *this;
}

ElementBufferObject ElementBufferObjectBuilder::Build(
    const VertexArrayObject& vao) {
  if (vao.ctx_id_ == 0 || vao.ctx_id_ != App::GetCtxId()) {
    return {};
  }
  GLuint ebo{kGlesInvalidBuffer};
  glGenBuffers(1, &ebo);

  vao.Bind();
  BindBuffer(kEboTarget, ebo);

  glBufferData(kEboTarget, size_, static_cast<const void*>(data_), usage_);

  return ElementBufferObject(ebo, vao.ctx_id_, size_, usage_);
}

ElementBufferObject::ElementBufferObject(GLuint ebo, std::int64_t ctx_id,
                                         std::size_t size, GLenum usage)
    : ebo_{ebo}, ctx_id_{ctx_id}, size_{size}, usage_{usage} {}

ElementBufferObject::ElementBufferObject(ElementBufferObject&& other)
    : ElementBufferObject() {
  std::swap(ebo_, other.ebo_);
  std::swap(ctx_id_, other.ctx_id_);
  std::swap(size_, other.size_);
  std::swap(usage_, other.usage_);
}

ElementBufferObject& ElementBufferObject::operator=(
    ElementBufferObject&& other) {
  if (&other == this) return *this;
  if (ebo_ != kGlesInvalidBuffer && ctx_id_ == App::GetCtxId()) {
    glDeleteBuffers(1, &ebo_);
  }

  ebo_ = other.ebo_;
  other.ebo_ = kGlesInvalidBuffer;

  ctx_id_ = other.ctx_id_;
  other.ctx_id_ = 0;

  size_ = other.size_;
  other.size_ = 0;

  usage_ = other.usage_;
  other.usage_ = kDefaultUsage;

  return *this;
}

ElementBufferObject::~ElementBufferObject() {
  if (ebo_ != kGlesInvalidBuffer && ctx_id_ == App::GetCtxId()) {
    glDeleteBuffers(1, &ebo_);
  }
}

GLuint ElementBufferObject::ebo() const { return ebo_; }

GLenum ElementBufferObject::usage() const { return usage_; }

}  // namespace Simple3D::Internal
