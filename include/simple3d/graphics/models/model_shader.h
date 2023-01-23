/** \~Russian
 * \file
 * \brief Класс Simple3D::ModelShader.
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_SHADER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_SHADER_H_

#include <simple3d/graphics/internal/gles_shader.h>
#include <simple3d/graphics/shader.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <unordered_set>

namespace Simple3D {

/** \~Russian
 * \class ModelShader
 * \brief Шейдер для моделей, использующих Simple3D::Internal::Vertex.
*/
class ModelShader : public IShader {
 public:
  /** \~Russian
   * \brief Создает шейдер.
   * 
   * Создание ModelShader при неинициализированном контексте - UB.
  */
  ModelShader();
  ModelShader(const ModelShader&) = delete;
  ModelShader(ModelShader&&) = delete;
  ModelShader& operator=(const ModelShader&) = delete;
  ModelShader& operator=(ModelShader&&) = delete;
  ~ModelShader() override = default;

  void Use() const override;
  void SetView(const glm::mat4& view) override;
  void SetProj(const glm::mat4& proj) override;
  void SetViewPos(const glm::vec3& view_pos) override;

  void SetDirectionalLights(
      const std::unordered_set<std::shared_ptr<DirectionalLight>>&) override;
  void SetAmbientLight(const glm::vec3&) override;

 private:
  Internal::GlesShader shader_{};
  glm::mat4 view_{0.0f};
  glm::mat4 proj_{0.0f};
  glm::vec3 view_pos_{0.0f};

  std::unordered_set<std::shared_ptr<DirectionalLight>> directional_lights_{};
  glm::vec3 ambient_light_{};
};

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_SHADER_H_
