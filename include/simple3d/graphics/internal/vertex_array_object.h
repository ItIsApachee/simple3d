#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_ARRAY_OBJECT_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_ARRAY_OBJECT_H_

#include <glad/gles2.h>
#include <simple3d/graphics/internal/misc.h>
#include <simple3d/types.h>

#include <cstdint>

namespace Simple3D::Internal {

class VertexArrayObject {
 public:
  friend class ElementBufferObjectBuilder;

  VertexArrayObject();
  VertexArrayObject(VertexArrayObject&&);
  VertexArrayObject& operator=(VertexArrayObject&&);
  VertexArrayObject(const VertexArrayObject&) = delete;
  VertexArrayObject& operator=(const VertexArrayObject&) = delete;
  ~VertexArrayObject();

  // TODO(apachee):
  // [ ] a way to define an array of generic vertex attribute data

  void BindEbo(const ElementBufferObject&) const;

  void Bind() const;
  static void Unbind();

 private:
  // bind optimization:
  // since there's only one render thread
  // we can statically store active VAO
  // to reduce the amount of binding calls
  static GLuint active_vao_;

  GLuint vao_{kGlesDefaultVao};
  std::int64_t ctx_id_{0};
};

}  // namespace Simple3D::Internal

#endif  //  INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_VERTEX_ARRAY_OBJECT_H_
