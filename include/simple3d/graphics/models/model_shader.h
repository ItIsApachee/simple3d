#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_SHADER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_SHADER_H_

#include <glm/mat4x4.hpp>

#include <simple3d/graphics/internal/shader.h>

namespace Simple3D {



// singletone of shader program used by primitives
class ModelShader {
 public:
  static ModelShader& GetInstance();

  ModelShader(const ModelShader&) = delete;
  ModelShader(ModelShader&&) = delete;
  ModelShader& operator=(const ModelShader&) = delete;
  ModelShader& operator=(ModelShader&&) = delete;
  ~ModelShader() = default;

  static void Init();
  static void Use();

  // FIXME(apachee): make static
  const Internal::Shader& shader() const;

 private:
  ModelShader();

  Internal::Shader shader_{};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_SHADER_H_