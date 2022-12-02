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

  assert(err.IsOk());
  // FIXME
  std::cerr << "is_ok: " << err.IsOk() << ", desc: " << err.description << std::endl;
}

void ModelShader::Init() {
  // is it going to be optimized?
  GetInstance();
}

void ModelShader::Use() {
  GetInstance().shader_.Use();
}

const Internal::Shader& ModelShader::shader() const {
  return shader_;
}



}  // namespace Simple3D