#include <glad/gles2.h>
#include <simple3d/graphics/models/primitive_renderer.h>

namespace Simple3D::Internal {

void PrimitiveInstance::BindAttributes() {
  constexpr auto model_layout_index = 2;
  constexpr auto model_row_size = sizeof(glm::vec4);
  constexpr auto model_rows = 4;

  for (int i = 0; i < model_rows; i++) {
    glVertexAttribPointer(
        model_layout_index + i, 4, GL_FLOAT, GL_FALSE,
        sizeof(Internal::PrimitiveInstance),
        reinterpret_cast<void*>(offsetof(Internal::PrimitiveInstance, model) +
                                model_row_size * i));
    glEnableVertexAttribArray(model_layout_index + i);
    glVertexAttribDivisor(model_layout_index + i, 1);
  }

  constexpr auto diffuse_color_layout_index = 10;
  glVertexAttribPointer(diffuse_color_layout_index, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Internal::PrimitiveInstance),
                        reinterpret_cast<void*>(offsetof(
                            Internal::PrimitiveInstance, diffuse_color)));
  glEnableVertexAttribArray(diffuse_color_layout_index);
  glVertexAttribDivisor(diffuse_color_layout_index, 1);

  constexpr auto specular_color_layout_index = 11;
  glVertexAttribPointer(specular_color_layout_index, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Internal::PrimitiveInstance),
                        reinterpret_cast<void*>(offsetof(
                            Internal::PrimitiveInstance, specular_color)));
  glEnableVertexAttribArray(specular_color_layout_index);
  glVertexAttribDivisor(specular_color_layout_index, 1);

  constexpr auto shininess_layout_index = 12;
  glVertexAttribPointer(shininess_layout_index, 1, GL_FLOAT, GL_FALSE,
                        sizeof(Internal::PrimitiveInstance),
                        reinterpret_cast<void*>(
                            offsetof(Internal::PrimitiveInstance, shininess)));
  glEnableVertexAttribArray(shininess_layout_index);
  glVertexAttribDivisor(shininess_layout_index, 1);
}

}  // namespace Simple3D::Internal
