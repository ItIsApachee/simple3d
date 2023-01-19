#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_

#include <filesystem>
#include <optional>
#include <memory>
#include <vector>
#include <unordered_map>

#include <glm/vec3.hpp>
#include <glad/gles2.h>

#include <simple3d/graphics/internal/gles_shader.h>
#include <simple3d/graphics/internal/vertex_buffer_object.h>
#include <simple3d/graphics/models/textured_model_shader.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/shader.h>

namespace Simple3D {



class Mesh;
class ModelRenderer;

class Model {
 public:
  friend ModelRenderer;

  static std::shared_ptr<Model> Load(const std::filesystem::path& model_path,
      std::optional<std::filesystem::path> textures_path = {});

  Model(const Model&) = delete;
  Model(Model&&) = default;
  Model& operator=(const Model&) = delete;
  Model& operator=(Model&&) = default;

 private:
  void Draw(const Internal::GlesShader&) const;

  Model() = default;
  std::vector<Mesh> meshes;
};

struct ModelInstance {
 public:
  using Renderer = ModelRenderer;
  glm::vec3 pos = glm::vec3(0.0f);
};

class ModelRenderer : public IRenderer {
 public:
  using Shader = TexturedModelShader;

  ModelRenderer() = default;
  ModelRenderer(const ModelRenderer&) = delete;
  ModelRenderer(ModelRenderer&&) = default;
  ModelRenderer& operator=(const ModelRenderer&) = delete;
  ModelRenderer& operator=(ModelRenderer&&) = default;;
  ~ModelRenderer() override = default;

  ModelInstance* Create(const std::shared_ptr<Model>& model,
      const glm::vec3& pos = glm::vec3(0.0f));

  void Draw(IShader&) override;

  // TODO(apachee): add optimization for instancing
  void NotifyUpdated(void*) override;
  void Destroy(void*) override;

 private:
  struct ModelGroup {
   public:
    std::shared_ptr<Model> model{};
    std::unordered_map<ModelInstance*,
        std::unique_ptr<ModelInstance>> instances{};

    // TODO(apachee): add instancing to increase performance
    // Internal::VertexBufferObject instances_vbo{};
  };

  std::unordered_map<Model*, ModelGroup> models_{};
  std::unordered_map<ModelInstance*, Model*> reverse_lookup_{};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_
