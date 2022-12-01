#include <simple3d/graphics/internal/vertex_array_object.h>

#include <utility>

#include <glad/gles2.h>

#include <simple3d/types.h>
#include <simple3d/graphics/internal/misc.h>
#include <simple3d/graphics/internal/element_buffer_object.h>

namespace Simple3D::Internal {



GLuint VertexArrayObject::active_vao_{kGlesDefaultVao};

VertexArrayObject::VertexArrayObject() {
  glGenVertexArrays(1, &vao_);
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& other)
    : vao_{kGlesDefaultVao} {
  std::swap(vao_, other.vao_);
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& other) {
  if (vao_ != kGlesDefaultVao)
    glDeleteVertexArrays(1, &vao_);
  vao_ = other.vao_;
  other.vao_ = kGlesDefaultVao;

  return *this;
}

VertexArrayObject::~VertexArrayObject() {
  if (vao_ != kGlesDefaultVao)
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



}