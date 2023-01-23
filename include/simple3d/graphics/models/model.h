/** \~Russian
 * \file
 * \brief Класс Simple3D::Model.
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_H_

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

/** \~Russian
 * \class Model
 * \brief Произвольная 3D модель.
 * 
 * Данный класс представляет собой произвольную
 * загруженную 3D модель.
*/
class Model {
 public:
  friend ModelRenderer;

  /** \~Russian
   * \brief Builder-метод для загрузки модели из файловой системы.
   * \param[in] model_path Путь, где лежит модель.
   * \param[in] textures_path Опциональный путь, где лежат текстуры модели.
   * По умолчанию используется тот же путь, что и model_path.
   * \return Возвращает загруженную модель.
   * 
   * \warning Неудачная загрузка модели вызывает терминирование программы.
   * \warning На данный момент не поддерживаются текстуры встроенные в файл модели.
   * \warning Загрузка модели при неинициализированном контексте - UB.
   * 
   * Для загрузки моделей используется библиотека assimp.
   * Подробнее о форматах поддерживаемых assimp:
   * https://github.com/assimp/assimp/blob/master/doc/Fileformats.md
  */
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

/** \~Russian
 * \class ModelInstance
 * \brief Экземпляр некоторой модели Model.
 * 
 * Чтобы добавить на сцену загруженную Model,
 * используется ModelRenderer, который создает
 * экземпляр заданной модели и дает возможность
 * изменять ее положение на сцене.
*/
struct ModelInstance {
 public:
  using Renderer = ModelRenderer;

  /** \~Russian
   * \brief Позиция экземпляра модели.
  */
  glm::vec3 pos = glm::vec3(0.0f);

  // TODO(apachee): add rotation
};

/** \~Russian
 * \class ModelRenderer
 * \brief Рендерер экземпляров Model.
*/
class ModelRenderer : public IRenderer {
 public:
  using Shader = TexturedModelShader;

  ModelRenderer() = default;
  ModelRenderer(const ModelRenderer&) = delete;
  ModelRenderer(ModelRenderer&&) = default;
  ModelRenderer& operator=(const ModelRenderer&) = delete;
  ModelRenderer& operator=(ModelRenderer&&) = default;;
  ~ModelRenderer() override = default;

  /** \~Russian
   * \brief Метод для создания экземпляра модели.
   * \param[in] model Модель, экземпляр которой создается.
   * \param[in] pos Положение модели. По умолчанию (0, 0, 0).
   * \return Указатель на созданный экземпляр модели.
   * 
   * Данная функция используется при вызове
   * Scene::Create<ModelInstance>(model, pos).
  */
  ModelInstance* Create(const std::shared_ptr<Model>& model,
      const glm::vec3& pos = glm::vec3(0.0f));

  void Draw(IShader*) override;

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

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_MODEL_H_
