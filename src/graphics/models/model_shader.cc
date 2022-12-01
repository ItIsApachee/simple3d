#include <simple3d/graphics/models/model_shader.h>

#include <cassert>

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
  shader = Internal::ShaderBuilder()
    .FragmentShaderSource(Internal::kFragmentShader)
    .VertexShaderSource(Internal::kVertexShader)
    .Build(&err);

  assert(err.IsOk());
}

void ModelShader::Init() {
  // is it going to be optimized?
  GetInstance();
}

void ModelShader::Use() {
  GetInstance().shader.Use();
}



}  // namespace Simple3D