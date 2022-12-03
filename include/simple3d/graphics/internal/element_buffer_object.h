#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_BUFFER_OBJECT_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_BUFFER_OBJECT_H_

#include <cstddef>

#include <glad/gles2.h>

#include <simple3d/types.h>
#include <simple3d/graphics/internal/misc.h>

namespace Simple3D::Internal {



class ElementBufferObjectBuilder {
 public:
  ElementBufferObjectBuilder() = default;
  ElementBufferObjectBuilder(const ElementBufferObjectBuilder&) = default;
  ElementBufferObjectBuilder(ElementBufferObjectBuilder&&) = default;
  ElementBufferObjectBuilder&
  operator=(const ElementBufferObjectBuilder&) = default;
  ElementBufferObjectBuilder&
  operator=(ElementBufferObjectBuilder&&) = default;
  ~ElementBufferObjectBuilder() = default;

  ElementBufferObjectBuilder& Data(std::size_t size, const std::byte* data = nullptr);
  ElementBufferObjectBuilder& Usage(GLenum usage);
  ElementBufferObject Build(const VertexArrayObject& vao);
 private:
  const std::byte* data_{nullptr};
  std::size_t size_{0};
  GLenum usage_{kDefaultUsage};
};



class ElementBufferObject {
// TODO(apachee): ability to change usage parameter of glBufferData
 public:
    friend ElementBufferObjectBuilder;

    ElementBufferObject() = default;
//   explicit ElementBufferObject(bool generate = false,
//       GLenum usage = kDefaultUsage);
//   ElementBufferObject(std::size_t size); TODO(apachee): preallocating buffer
//   ElementBufferObject(std::size_t size, std::uint32_t* data = nullptr,
//       GLenum usage = kDefaultUsage);
  ElementBufferObject(ElementBufferObject&&);
  ElementBufferObject& operator=(ElementBufferObject&&);
  ElementBufferObject(const ElementBufferObject&) = delete;
  ElementBufferObject& operator=(const ElementBufferObject&) = delete;
  ~ElementBufferObject();

  // TODO(apachee): add buffer manipulation (currently buffer is immutable)
  // deprecated: binding should be done through VAO
//   void Bind();
//   static void Unbind();

  GLuint ebo() const;
  GLenum usage() const;

 private:
  ElementBufferObject(GLuint ebo, std::size_t size, GLenum usage);

  GLuint ebo_{kGlesInvalidBuffer};  // handle for OpenGL buffer
  GLenum usage_{kDefaultUsage};  // usage of the buffer
  std::size_t size_{0};  // capacity of OpenGL buffer

  // maybe storing buffer is bad:
  // duplication of the same information
  // std::vector<std::uint32_t> buffer_{};
};



}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_BUFFER_OBJECT_H_
