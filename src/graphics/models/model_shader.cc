#include <simple3d/graphics/models/model_shader.h>

#include <cassert>
#include <iostream>

#include <simple3d/misc/error.h>
#include <simple3d/graphics/internal/shader_source.h>
#include <simple3d/graphics/internal/shader.h>

namespace Simple3D {



ModelShader& ModelShader::GetInstance() {
  static ModelShader model_shader{};
  return model_shader;
}

ModelShader::ModelShader() {
  Error err;
  shader_ = Internal::ShaderBuilder()
    .FragmentShaderSource(Internal::kFragmentShader)
    .VertexShaderSource(Internal::kVertexShader)
    .Build(&err);

  // FIXME
  std::cerr << "is_ok: " << err.IsOk() << ", desc: " << err.description << std::endl;
  assert(err.IsOk());
}

void ModelShader::Init() {
  // is it going to be optimized?
  GetInstance();
}

void ModelShader::Use() {
  GetInstance().shader_.Use();
}

void ModelShader::SetView(const glm::mat4& view) {
  ModelShader& inst = GetInstance();
  inst.Use();
  if (view != inst.view_) {
    inst.view_ = view;
    inst.shader().SetUniformMat4fv("view", view);
  }
}

void ModelShader::SetProj(const glm::mat4& proj) {
  ModelShader& inst = GetInstance();
  inst.Use();
  if (proj != inst.proj_) {
    inst.proj_ = proj;
    inst.shader().SetUniformMat4fv("projection", proj);
  }
}

const Internal::Shader& ModelShader::shader() const {
  return shader_;
}



}  // namespace Simple3D