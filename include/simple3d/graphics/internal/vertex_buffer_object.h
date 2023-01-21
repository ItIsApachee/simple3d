#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_BUFFER_OBJECT_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_BUFFER_OBJECT_H_

#include <glad/gles2.h>
#include <simple3d/graphics/internal/misc.h>

#include <cstddef>
#include <cstdint>

namespace Simple3D::Internal {

class VertexBufferObject {
 public:
  VertexBufferObject() = default;
  explicit VertexBufferObject(std::size_t size, const std::byte* data = nullptr,
                              GLenum usage = kDefaultUsage);
  VertexBufferObject(VertexBufferObject&&);
  VertexBufferObject& operator=(VertexBufferObject&&);
  VertexBufferObject(const VertexBufferObject&) = delete;
  VertexBufferObject& operator=(const VertexBufferObject&) = delete;
  ~VertexBufferObject();

  void SetData(std::size_t size, const std::byte* data = nullptr,
               GLenum usage = kDefaultUsage);
  void SubData(std::size_t offset, std::size_t size, const std::byte* data);

  void Bind() const;
  // static void Unbind();

  GLuint vbo() const;
  std::size_t size() const;
  GLenum usage() const;

 private:
  GLuint vbo_{kGlesInvalidBuffer};
  std::int64_t ctx_id_{0};
  GLenum usage_{kDefaultUsage};
  std::size_t size_{0};
};

}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_BUFFER_OBJECT_H_
