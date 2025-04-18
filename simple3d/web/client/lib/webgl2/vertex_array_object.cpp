#include <GLES3/gl3.h>
#include <simple3d/context/context.h>
#include <simple3d/gl/web/element_buffer_object.h>
#include <simple3d/gl/web/misc.h>
#include <simple3d/gl/web/vertex_array_object.h>
#include <simple3d/types.h>

#include <utility>

namespace Simple3D::Internal {

GLuint VertexArrayObject::active_vao_{kGlesDefaultVao};

VertexArrayObject::VertexArrayObject() : ctx_id_{App::GetCtxId()} {
  if (ctx_id_ != 0) glGenVertexArrays(1, &vao_);
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& other)
    : vao_{kGlesDefaultVao}, ctx_id_{0} {
  std::swap(vao_, other.vao_);
  std::swap(ctx_id_, other.ctx_id_);
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& other) {
  if (&other == this) return *this;
  if (vao_ != kGlesDefaultVao && ctx_id_ == App::GetCtxId())
    glDeleteVertexArrays(1, &vao_);
  vao_ = other.vao_;
  other.vao_ = kGlesDefaultVao;
  ctx_id_ = other.ctx_id_;
  other.ctx_id_ = 0;

  return *this;
}

VertexArrayObject::~VertexArrayObject() {
  if (vao_ != kGlesDefaultVao && ctx_id_ == App::GetCtxId())
    glDeleteVertexArrays(1, &vao_);
}

void VertexArrayObject::BindEbo(const ElementBufferObject& ebo) const {
  constexpr GLenum kEboTarget = GL_ELEMENT_ARRAY_BUFFER;
  Bind();
  BindBuffer(kEboTarget, ebo.ebo());
}

void VertexArrayObject::Bind() const {
  if (active_vao_ != vao_) {
    glBindVertexArray(vao_);
    active_vao_ = vao_;
  }
}

void VertexArrayObject::Unbind() {
  if (active_vao_ != kGlesDefaultVao) {
    glBindVertexArray(kGlesDefaultVao);
    active_vao_ = kGlesDefaultVao;
  }
}

}  // namespace Simple3D::Internal
