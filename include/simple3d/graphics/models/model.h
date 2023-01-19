#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_

#include <filesystem>
#include <optional>
#include <memory>
#include <vector>

#include <simple3d/graphics/internal/gles_shader.h>

namespace Simple3D {

class Mesh;

class Model {
 public:
  static std::shared_ptr<Model> Load(const std::filesystem::path& model_path,
      std::optional<std::filesystem::path> textures_path = {});

  Model(const Model&) = delete;
  Model(Model&&) = default;
  Model& operator=(const Model&) = delete;
  Model& operator=(Model&&) = default;

  void Draw(const Internal::GlesShader&) const;

 private:
  Model() = default;
  std::vector<Mesh> meshes;
};

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_
