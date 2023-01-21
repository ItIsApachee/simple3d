#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_BUFFER_OBJECT_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_BUFFER_OBJECT_H_

#include <glad/gles2.h>
#include <simple3d/graphics/internal/misc.h>
#include <simple3d/types.h>

#include <cstddef>
#include <cstdint>

namespace Simple3D::Internal {

class ElementBufferObjectBuilder {
 public:
  ElementBufferObjectBuilder() = default;
  ElementBufferObjectBuilder(const ElementBufferObjectBuilder&) = default;
  ElementBufferObjectBuilder(ElementBufferObjectBuilder&&) = default;
  ElementBufferObjectBuilder& operator=(const ElementBufferObjectBuilder&) =
      default;
  ElementBufferObjectBuilder& operator=(ElementBufferObjectBuilder&&) = default;
  ~ElementBufferObjectBuilder() = default;

  ElementBufferObjectBuilder& Data(std::size_t size,
                                   const std::byte* data = nullptr);
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
  ElementBufferObject(ElementBufferObject&&);
  ElementBufferObject& operator=(ElementBufferObject&&);
  ElementBufferObject(const ElementBufferObject&) = delete;
  ElementBufferObject& operator=(const ElementBufferObject&) = delete;
  ~ElementBufferObject();

  GLuint ebo() const;
  GLenum usage() const;

 private:
  ElementBufferObject(GLuint ebo, std::int64_t ctx_id, std::size_t size, GLenum usage);

  GLuint ebo_{kGlesInvalidBuffer};  // handle for OpenGL buffer
  GLenum usage_{kDefaultUsage};     // usage of the buffer
  std::size_t size_{0};             // capacity of OpenGL buffer
  std::int64_t ctx_id_{0};
};

}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_BUFFER_OBJECT_H_
