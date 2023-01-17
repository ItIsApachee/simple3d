#include <simple3d/graphics/models/model_shader.h>

#include <cassert>
#include <iostream>

#include <simple3d/misc/error.h>
#include <simple3d/graphics/internal/shader_source.h>
#include <simple3d/graphics/internal/shader.h>

namespace Simple3D {



// FIXME: make string constants for shader fields
ModelShader::ModelShader() {
  Error err;
  shader_ = Internal::GlesShaderBuilder()
    .FragmentShaderSource(Internal::kFragmentShader)
    .VertexShaderSource(Internal::kVertexShader)
    .Build(&err);

  // FIXME
  std::cerr << "is_ok: " << err.IsOk() << ", desc: " << err.description << std::endl;
  assert(err.IsOk());
}

void ModelShader::Use() {
  shader_.Use();
}

void ModelShader::SetView(const glm::mat4& view) {
  Use();
  if (view != view_) {
    view_ = view;
    shader_.SetUniformMat4fv("view", view);
  }
}

void ModelShader::SetProj(const glm::mat4& proj) {
  Use();
  if (proj != proj_) {
    proj_ = proj;
    shader_.SetUniformMat4fv("projection", proj);
  }
}

void ModelShader::SetViewPos(const glm::vec3& view_pos) {
  Use();
  if (view_pos != view_pos_) {
    view_pos_ = view_pos;
    shader_.SetUniform3fv("view_pos", view_pos);
  }
}



}  // namespace Simple3D