#include <simple3d/graphics/internal/element_buffer_object.h>

#include <utility>
#include <cstddef>

#include <glad/gles2.h>

#include <simple3d/graphics/internal/misc.h>
#include <simple3d/graphics/internal/vertex_array_object.h>

namespace Simple3D::Internal {



constexpr auto kEboTarget = GL_ELEMENT_ARRAY_BUFFER;

ElementBufferObjectBuilder& ElementBufferObjectBuilder::Data(std::size_t size,
    std::byte* data) {
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
  GLuint ebo{kGlesInvalidBuffer};
  glGenBuffers(1, &ebo);

  vao.Bind();
  BindBuffer(kEboTarget, ebo);

  glBufferData(
      kEboTarget, size_, static_cast<void*>(data_), usage_);
  
  return ElementBufferObject(ebo, size_, usage_);
}

// deprecated: no reason to use other than default constructor
// ElementBufferObject::ElementBufferObject(bool generate, GLenum usage)
//     : size_{0}, usage_{usage} {
//   if (generate)
//     glGenBuffers(1, &ebo_);
// }

// deprecated: can't bind without an active VAO
// solution: create EBO using builder (or something similar), that binds VAO
// at creation of EBO
// ElementBufferObject::ElementBufferObject(
//     std::size_t size, std::uint32_t* data, GLenum usage)
//     : size_{size}, usage_{usage} {
//   glGenBuffers(1, &ebo_);
//   static_assert(false, "can't bind EBO when VAO isn't bound");
//   Bind();
//   glBufferData(
//     kEboTarget, size, static_cast<void*>(data), usage_);
//   Unbind();
// }

// deprecated: copy ctor overcomplicates everything
// reason: VAO should be bound for any EBO binding
// ElementBufferObject::ElementBufferObject(const ElementBufferObject& other)
//     : size_{other.size_}, usage_{other.usage_} {
//   if (other.ebo_ == kGlesInvalidBuffer) {
//     // constructing from empty buffer creates empty buffer
//     ebo_ = kGlesInvalidBuffer;
//     return;
//   }

//   // other is not an empty buffer
//   glGenBuffers(1, &ebo_);
//   static_assert(false, "can't bind EBO when VAO isn't bound");
//   Bind();
//   glBufferData(kEboTarget, size_, nullptr, usage_);
//   Unbind();

//   CopyBuffer(other.ebo_, ebo_, 0, 0, size_);
// }

ElementBufferObject::ElementBufferObject(GLuint ebo, std::size_t size,
    GLenum usage) : ebo_{ebo}, size_{size}, usage_{usage} {}

ElementBufferObject::ElementBufferObject(ElementBufferObject&& other)
    : ElementBufferObject() {
  std::swap(ebo_, other.ebo_);
  std::swap(size_, other.size_);
  std::swap(usage_, other.usage_);
}

// deprecated: copy assignment overcomplicates everything
// reason: VAO should be bound for any EBO binding
// ElementBufferObject& ElementBufferObject::operator=(
//     const ElementBufferObject& other) {
//   if (&other == this)
//     return *this;
//   if (other.ebo_ == kGlesInvalidBuffer) {
//     // copying empty buffer creates empty buffer
//     // and destroys previous buffer if `this` is not an empty buffer
//     if (ebo != kGlesInvalidBuffer) {
//       glDeleteBuffers(1, &ebo_);
//       ebo_ = kGlesInvalidBuffer;
//     }
//     size_ = other.size_;
//     usage_ = other.usage_;
//   }

//   // other is NOT an empty buffer

//   // reuse existing buffer instead of deleting & generating
//   if (ebo_ == kGlesInvalidBuffer) {
//     glGenBuffers(1, &ebo_);
//   }
//   if (size_ != other.size_ || usage_ != other.usage_) {
//     static_assert(false, "can't bind EBO when VAO isn't bound");
//     Bind();
//     glBufferData(
//       kEboTarget, other.size_, nullptr, other.usage_);
//     Unbind();
//     size_ = other.size_;
//     usage_ = other.usage_;
//   }

//   CopyBuffer(other.ebo_, ebo_, 0, 0, size_);
//   return *this;
// }

ElementBufferObject& ElementBufferObject::operator=(
    ElementBufferObject&& other) {
  if (&other == this)
    return *this;
  if (ebo_ != kGlesInvalidBuffer) {
    glDeleteBuffers(1, &ebo_);
  }

  ebo_ = other.ebo_;
  other.ebo_ = kGlesInvalidBuffer;

  size_ = other.size_;
  other.size_ = 0;

  usage_ = other.usage_;
  other.usage_ = kDefaultUsage;

  return *this;
}

ElementBufferObject::~ElementBufferObject() {
  if (ebo_ != kGlesInvalidBuffer) {
    glDeleteBuffers(1, &ebo_);
  }
}

// void ElementBufferObject::Bind() {
//   static_assert(false, "can't bind EBO when VAO isn't bound");
//   if (ebo_ != kGlesInvalidBuffer)
//     BindBuffer(kEboTarget, ebo_);
// }

// void ElementBufferObject::Unbind() {
//   UnbindBuffer(kEboTarget);
// }

GLuint ElementBufferObject::ebo() const {
  return ebo_;
}

GLenum ElementBufferObject::usage() const {
  return usage_;
}



}  // namespace Simple3D::Internal
