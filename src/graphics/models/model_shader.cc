#include <simple3d/graphics/internal/gles_shader.h>
#include <simple3d/graphics/internal/shader_source.h>
#include <simple3d/graphics/models/model_shader.h>
#include <simple3d/misc/error.h>

#include <cassert>
#include <iostream>
#include <string>

namespace Simple3D {

// FIXME: make string constants for shader fields
ModelShader::ModelShader() {
  Error err;
  shader_ = Internal::GlesShaderBuilder()
                .FragmentShaderSource(Internal::kDefaultFragmentShader)
                .VertexShaderSource(Internal::kDefaultVertexShader)
                .Build(&err);

  // FIXME
  std::cerr << "is_ok: " << err.IsOk() << ", desc: " << err.description
            << std::endl;
  assert(err.IsOk());
}

void ModelShader::Use() const { shader_.Use(); }

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

void ModelShader::SetDirectionalLights(
    const std::unordered_set<std::shared_ptr<DirectionalLight>>& dir_lights) {
  Use();
  if (directional_lights_ != dir_lights) {
    directional_lights_ = dir_lights;

    int directional_light_count = 0;
    auto it = dir_lights.begin();
    while (directional_light_count < kMinimumDirectionalLights &&
           it != dir_lights.end()) {
      std::string name = "directional_light[";
      name += std::to_string(directional_light_count++);
      name += "]";

      shader_.SetUniform3fv(name + ".direction", (*it)->direction);
      shader_.SetUniform3fv(name + ".diffuse", (*it)->diffuse);
      shader_.SetUniform3fv(name + ".specular", (*it)->specular);

      it++;
    }

    shader_.SetUniform1i("directional_light_count", directional_light_count);
  }
}
void ModelShader::SetAmbientLight(const glm::vec3& ambient_light) {
  Use();
  if (ambient_light_ != ambient_light) {
    ambient_light_ = ambient_light;
    shader_.SetUniform3fv("ambient_light", ambient_light);
  }
}

}  // namespace Simple3D
