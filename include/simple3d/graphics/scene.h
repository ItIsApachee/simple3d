/** \file
 * \brief Definition of Simple3D::Scene
*/

#ifndef INCLUDE_SIMPLE3D_GRAPHICS_SCENE_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_SCENE_H_

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>
#include <memory>
#include <utility>
#include <typeindex>

#include <glm/vec3.hpp>

#include <simple3d/types.h>
#include <simple3d/graphics/view.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/model.h>
#include <simple3d/graphics/camera.h>
#include <simple3d/graphics/shader.h>
#include <simple3d/graphics/shader_storage.h>
#include <simple3d/graphics/light.h>

namespace Simple3D {



const float kDefaultAmbientLight = 0.1f;

// definition
/** \class Scene simple3d/graphics/scene.h
 * \brief Container for light sources, and cameras.
*/
class Scene {
 public:
  friend class View;
  Scene();
  // TODO(apachee): add move constructor/assignment
  Scene(const Scene&) = delete;
  Scene(Scene&&) = delete;
  Scene& operator=(const Scene&) = delete;
  Scene& operator=(Scene&&) = delete;
  ~Scene() = default;

  void SetCamera(std::shared_ptr<ICamera> camera);

  // TODO(apachee): add ability to initialize renderer like
  // Scene::AddRenerer<R>(R&& renderer)

  // usage example: group objects in separate renderers with different
  // configuration
  // template <typename P, typename... Args>
  // Model<P> Create(Args... args);
  template <typename M, typename... Args>
  auto Create(Args&&... args)
    -> std::enable_if_t<
      std::is_same_v<
        decltype(
          std::declval<typename M::Renderer>().template Create<M>(
            std::forward<Args>(args)...)),
        M*>,
      Model<M>>;

  template <typename M, typename... Args>
  auto Create(Args&&... args)
    -> std::enable_if_t<
      std::is_same_v<
        decltype(
          std::declval<typename M::Renderer>().Create(
            std::forward<Args>(args)...)),
        M*>,
      Model<M>>;

  template <typename M, typename R, typename... Args>
  auto Create(Args&&... args)
    -> std::enable_if_t<
      std::is_same_v<
        decltype(
          std::declval<R>().template Create<M>(
            std::forward<Args>(args)...)),
        M*>,
      Model<M>>;

  template <typename M, typename R, typename... Args>
  auto Create(Args&&... args)
    -> std::enable_if_t<
      std::is_same_v<
        decltype(
          std::declval<R>().Create(
            std::forward<Args>(args)...)),
        M*>,
      Model<M>>;

  void AddDirectionalLight(const std::shared_ptr<DirectionalLight>& dir_light);
  void RemoveDirectionalLight(const std::shared_ptr<DirectionalLight>& dir_light);
  void SetAmbientLight(const glm::vec3& light);

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
  glm::vec3 ambient_light_ = glm::vec3(kDefaultAmbientLight);
};



// implementation
template <typename R>
std::shared_ptr<IRenderer>& Scene::GetIRendererShared() {
  using Shader = R::Shader;

  std::type_index shader_type = typeid(Shader);
  std::type_index renderer_type = typeid(R);

  auto renderers_it = renderers_.find(shader_type);
  if (renderers_it == renderers_.end()) {
    auto& cell = ShaderCell{
      ShaderStorage::GetInstance().GetShader<Shader>(),
      {}
    };
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
auto Scene::Create(Args&&... args)
  -> std::enable_if_t<
    std::is_same_v<
      decltype(
        std::declval<typename M::Renderer>().template Create<M>(
          std::forward<Args>(args)...)),
      M*>,
    Model<M>> {
    return Scene::Create<M, typename M::Renderer>(args...);
}

template <typename M, typename... Args>
auto Scene::Create(Args&&... args)
  -> std::enable_if_t<
    std::is_same_v<
      decltype(
        std::declval<typename M::Renderer>().Create(
          std::forward<Args>(args)...)),
      M*>,
    Model<M>> {
    return Scene::Create<M, typename M::Renderer>(args...);
}

template <typename M, typename R, typename... Args>
auto Scene::Create(Args&&... args)
  -> std::enable_if_t<
    std::is_same_v<
      decltype(
        std::declval<R>().template Create<M>(
          std::forward<Args>(args)...)),
      M*>,
    Model<M>> {
  using Renderer = R;
  // auto& renderer = GetRendererRef<R>();
  auto renderer = GetIRendererShared<R>();
  R& renderer_ref = *dynamic_cast<R*>(renderer.get());

  return Model{
    renderer_ref.template Create<M>(std::forward<Args>(args)...),
    std::move(renderer)
  };
}

template <typename M, typename R, typename... Args>
auto Scene::Create(Args&&... args)
  -> std::enable_if_t<
    std::is_same_v<
      decltype(
        std::declval<R>().Create(
          std::forward<Args>(args)...)),
      M*>,
    Model<M>> {
  using Renderer = R;
  // auto& renderer = GetRendererRef<R>();
  auto renderer = GetIRendererShared<R>();
  R& renderer_ref = *dynamic_cast<R*>(renderer.get());

  return Model{
    renderer_ref.Create(std::forward<Args>(args)...),
    std::move(renderer)
  };
}



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_SCENE_H_
