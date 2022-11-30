#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_ARRAY_BUFFER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_ARRAY_BUFFER_H_

#include <cstdint>
#include <cstddef>

#include <glad/gles2.h>

#include <simple3d/graphics/internal/misc.h>

namespace Simple3D::Internal {



constexpr auto kDefaultUsage = GL_STATIC_DRAW;

class ElementArrayBuffer {
// TODO(apachee): ability to change usage parameter of glBufferData
 public:
  explicit ElementArrayBuffer(bool generate = false, GLenum usage = kDefaultUsage);
  // ElementArrayBuffer(std::size_t size); TODO(apachee): preallocating buffer
  ElementArrayBuffer(std::size_t size, std::uint32_t* data, GLenum usage = kDefaultUsage);
  ElementArrayBuffer(const ElementArrayBuffer&);
  ElementArrayBuffer(ElementArrayBuffer&&);
  ElementArrayBuffer& operator=(const ElementArrayBuffer&);
  ElementArrayBuffer& operator=(ElementArrayBuffer&&);
  ~ElementArrayBuffer();

  // TODO(apachee): add buffer manipulation (currently buffer is immutable)
  void Bind();
  static void Unbind();

  GLuint ebo();

 private:
  GLuint ebo_{kGlesInvalidBuffer};  // handle for OpenGL buffer
  GLenum usage_{kDefaultUsage};  // usage of the buffer
  std::size_t size_{0};  // capacity of OpenGL buffer

  // maybe storing buffer is bad:
  // duplication of the same information
  // std::vector<std::uint32_t> buffer_{};
};



}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_ELEMENT_ARRAY_BUFFER_H_
