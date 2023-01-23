/** \~Russian
 * \file
 * \brief Объект Simple3D::Scene
 */

#ifndef INCLUDE_SIMPLE3D_GRAPHICS_SCENE_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_SCENE_H_

#include <simple3d/graphics/camera.h>
#include <simple3d/graphics/light.h>
#include <simple3d/graphics/model_handle.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/shader.h>
#include <simple3d/graphics/shader_storage.h>
#include <simple3d/graphics/view.h>
#include <simple3d/types.h>

#include <glm/vec3.hpp>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Simple3D {

/** \~Russian
 * \brief Фоновое освещение сцены по умолчанию.
*/
const float kDefaultAmbientLight = 0.1f;

// definition
/** \~Russian
 * \class Scene
 * \brief Контейнер объектов на сцене.
 * 
 * Сцена предоставляет собой объект, хранящий отрисовываемые
 * объекты, их относительное положение.
 * Сцена также хранит наблюдателя и параметры освещения.
 */
class Scene {
 public:
  friend class View;

  Scene();
  Scene(const Scene&) = delete;
  Scene(Scene&&);
  Scene& operator=(const Scene&) = delete;
  Scene& operator=(Scene&&);
  ~Scene() = default;

  // FIXME: add const ref
  /** \~Russian
   * \brief Метод для установки наблюдателя.
   * \param[in] camera Наблюдатель.
  */
  void SetCamera(std::shared_ptr<ICamera> camera);

  // TODO(apachee): add ability to initialize renderer like
  // Scene::AddRenerer<R>(R&& renderer)

  /** \~Russian
   * \brief Создание объекта типа M с помощью args.
   * \tparam M Тип создаваемой модели. Данная перегрузка использует
   * M\::Renderer как тип рендерера, используемый для создания M.
   * \tparam Args Типы аргументов для создания модели.
   * \param args Аргументы для создания модели.
   * 
   * Создает объект типа M, передавая args рендереру M\::Renderer.
   * Требования к типу M\::Renderer:
   * - Реализация интерфейса Simple3D::IRenderer.
   * - Имеет тип или псевдоним типа M\::Renderer\::Shader.
   * - Имеет метод M* M\::Renderer::Create.
   * Аргументы args передаются прямой передачей в
   * M* M\::Renderer::Create для создания объекта.
   * 
   * Требования к типу M\::Renderer\::Shader:
   * - Реализация интерфейса Simple3D::IShader.
   * 
   * У данного метода есть перегрузка с явным указанием
   * типа рендерера R. С помощью нее можно, например,
   * создавать существующие в этой библиотеки примитивы
   * (например, Simple3D::Cuboid), используя другие рендереры.
  */
  template <typename M, typename... Args>
  auto Create(Args&&... args) -> std::enable_if_t<
      std::is_same_v<decltype(std::declval<typename M::Renderer>().Create(
                         std::forward<Args>(args)...)),
                     M*>,
      ModelHandle<M>>;

  /** \~Russian
   * \brief Создание объекта типа M с помощью args и рендерера R.
   * \tparam M Тип создаваемой модели.
   * \tparam R Тип рендерера, используемого для создания модели.
   * \tparam Args Типы аргументов для создания модели.
   * \param args Аргументы для создания модели.
   * 
   * Создает объект типа M, передавая args рендереру R.
   * Требования к типу R:
   * - Реализация интерфейса Simple3D::IRenderer.
   * - Имеет тип или псевдоним типа R\::Shader.
   * - Имеет метод M* R\::Create.
   * Аргументы args передаются прямой передачей в
   * M* M\::Renderer::Create для создания объекта.
   * 
   * Требования к типу R\::Shader:
   * - Реализация интерфейса Simple3D::IShader.
   * 
   * Используя данную перегрузку можно, например, использовать свою реализацию
   * рендерера для некоторого примитива.
  */
  template <typename M, typename R, typename... Args>
  auto Create(Args&&... args) -> std::enable_if_t<
      std::is_same_v<
          decltype(std::declval<R>().Create(std::forward<Args>(args)...)), M*>,
      ModelHandle<M>>;

  /** \~Russian
   * \brief Функция для добавления источника направленного света.
   * \param[in] dir_light Источник направленного света.
  */
  void AddDirectionalLight(const std::shared_ptr<DirectionalLight>& dir_light);

  /** \~Russian
   * \brief Функция для удаления источника направленного света.
   * \param[in] dir_light Источник направленного света.
  */
  void RemoveDirectionalLight(
      const std::shared_ptr<DirectionalLight>& dir_light);

  // deprecated
  // void SetAmbientLight(const glm::vec3& light);
  // void SetBackgroundColor(const glm::vec3& bg_color);

  /** \~Russian
   * \brief Цвет фонового освещения объектов.
  */
  glm::vec3 ambient_light = glm::vec3(kDefaultAmbientLight);

  /** \~Russian
   * \brief Цвет фона сцены.
  */
  glm::vec3 background_color = glm::vec3(0.0f);

 private:
  struct ShaderCell {
   public:
    std::weak_ptr<IShader> shader{};
    std::unordered_map<std::type_index, std::shared_ptr<IRenderer>> renderers{};
  };

  template <typename R>
  std::shared_ptr<IRenderer>& GetIRendererShared();

  template <typename R>
  R& GetRendererRef();

  std::unordered_map<std::type_index, ShaderCell> renderers_{};

  std::shared_ptr<ICamera> active_camera_{};

  std::unordered_set<std::shared_ptr<DirectionalLight>> directional_lights_{};
};

// implementation
template <typename R>
std::shared_ptr<IRenderer>& Scene::GetIRendererShared() {
  using Shader = R::Shader;

  std::type_index shader_type = typeid(Shader);
  std::type_index renderer_type = typeid(R);

  auto renderers_it = renderers_.find(shader_type);
  if (renderers_it == renderers_.end()) {
    auto& cell =
        ShaderCell{ShaderStorage::GetInstance().GetShader<Shader>(), {}};
    renderers_it = renderers_.emplace(shader_type, std::move(cell)).first;
  }

  auto& shader_renderers = renderers_it->second.renderers;
  auto it = shader_renderers.find(renderer_type);
  if (it == shader_renderers.end()) {
    it = shader_renderers.emplace(renderer_type, std::make_shared<R>()).first;
  }

  auto& renderer_ptr = it->second;
  return renderer_ptr;
}

template <typename R>
R& Scene::GetRendererRef() {
  return *dynamic_cast<R*>(GetIRendererShared<R>().get());
}

template <typename M, typename... Args>
auto Scene::Create(Args&&... args) -> std::enable_if_t<
    std::is_same_v<decltype(std::declval<typename M::Renderer>().Create(
                       std::forward<Args>(args)...)),
                   M*>,
    ModelHandle<M>> {
  return Scene::Create<M, typename M::Renderer>(args...);
}

template <typename M, typename R, typename... Args>
auto Scene::Create(Args&&... args) -> std::enable_if_t<
    std::is_same_v<
        decltype(std::declval<R>().Create(std::forward<Args>(args)...)), M*>,
    ModelHandle<M>> {
  using Renderer = R;
  // auto& renderer = GetRendererRef<R>();
  auto renderer = GetIRendererShared<R>();
  R& renderer_ref = *dynamic_cast<R*>(renderer.get());

  return ModelHandle{renderer_ref.Create(std::forward<Args>(args)...),
                     std::move(renderer)};
}

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_SCENE_H_
