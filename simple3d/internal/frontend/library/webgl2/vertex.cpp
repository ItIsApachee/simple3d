#include <simple3d/graphics/internal/vertex.h>

#include <GLES3/gl3.h>

namespace Simple3D::Internal {



void Vertex::BindAttributes() {
  glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      reinterpret_cast<void*>(offsetof(Vertex, position)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      reinterpret_cast<void*>(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);
}

void TexturedVertex::BindAttributes() {
  glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
      reinterpret_cast<void*>(offsetof(TexturedVertex, position)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
      reinterpret_cast<void*>(offsetof(TexturedVertex, normal)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(
      2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
      reinterpret_cast<void*>(offsetof(TexturedVertex, texture_coords)));
  glEnableVertexAttribArray(2);
}



}  // namespace Simple3D::Internal
