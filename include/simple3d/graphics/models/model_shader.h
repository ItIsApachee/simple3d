#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_SHADER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_SHADER_H_

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <simple3d/graphics/internal/shader.h>
#include <simple3d/graphics/shader.h>

namespace Simple3D {



// singletone of shader program used by primitives
class ModelShader : public IShader {
 public:
  ModelShader();
  ModelShader(const ModelShader&) = delete;
  ModelShader(ModelShader&&) = delete;
  ModelShader& operator=(const ModelShader&) = delete;
  ModelShader& operator=(ModelShader&&) = delete;
  ~ModelShader() override = default;

  void Use() override;
  void SetView(const glm::mat4& view) override;
  void SetProj(const glm::mat4& proj) override;
  void SetViewPos(const glm::vec3& view_pos) override;

 private:
  Internal::Shader shader_{};
  glm::mat4 view_{0.0f};
  glm::mat4 proj_{0.0f};
  glm::vec3 view_pos_{0.0f};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_SHADER_H_