#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_BUFFER_OBJECT_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_BUFFER_OBJECT_H_

#include <cstddef>

#include <glad/gles2.h>

#include <simple3d/graphics/internal/misc.h>

namespace Simple3D::Internal {




class VertexBufferObject {
 public:
  VertexBufferObject() = default;
  // explicit VertexBufferObject(bool generate = false,
  //                             GLenum usage = kDefaultUsage);
  explicit VertexBufferObject(std::size_t size, std::byte* data = nullptr,
      GLenum usage = kDefaultUsage);
  VertexBufferObject(VertexBufferObject&&);
  VertexBufferObject& operator=(VertexBufferObject&&);
  VertexBufferObject(const VertexBufferObject&) = delete;
  VertexBufferObject& operator=(const VertexBufferObject&) = delete;
  ~VertexBufferObject();

  void Bind();
  static void Unbind();

  GLuint vbo();
  GLenum usage();

 private:
  GLuint vbo_{kGlesInvalidBuffer};
  GLenum usage_{kDefaultUsage};
  std::size_t size_{0};
};



}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_BUFFER_OBJECT_H_
