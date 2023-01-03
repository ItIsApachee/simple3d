/** \file
 * \brief Definition of Simple3D::Scene
*/

#ifndef INCLUDE_SIMPLE3D_GRAPHICS_SCENE_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_SCENE_H_

#include <vector>
#include <type_traits>
#include <memory>
#include <utility>

#include <simple3d/types.h>
#include <simple3d/graphics/view.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/model.h>
#include <simple3d/graphics/camera.h>

namespace Simple3D {



// definition
/** \class Scene simple3d/graphics/scene.h
 * \brief Container for light sources, and cameras.
*/
class Scene {
 public:
  friend class View;
  Scene();
  Scene(const Scene&) = delete;
  Scene(Scene&&) = default;
  Scene& operator=(const Scene&) = delete;
  Scene& operator=(Scene&&) = default;
  ~Scene();

  void SetCamera(std::shared_ptr<ICamera> camera);

  // TODO(apachee): add ability to initialize renderer like
  // Scene::AddRenerer<R>(R&& renderer)

  // TODO(apachee): add ability to have multiple instances of renderers of the
  // same type

  // usage example: group objects in separate renderers with different
  // configuration
  // template <typename P, typename... Args>
  // Model<P> Create(Args... args);
  template <typename M, typename... Args>
  auto Create(Args... args)
    -> std::enable_if_t<
      std::is_same_v<
        decltype(
          std::declval<typename M::Renderer>().template Create<M>(args...)),
        M*>,
      Model<M>>;

  template <typename M, typename... Args>
  auto Create(Args... args)
    -> std::enable_if_t<
      std::is_same_v<
        decltype(
          std::declval<typename M::Renderer>().Create(args...)),
        M*>,
      Model<M>>;

  template <typename M, typename R, typename... Args>
  auto Create(Args... args)
    -> std::enable_if_t<
      std::is_same_v<
        decltype(
          std::declval<R>().template Create<M>(args...)),
        M*>,
      Model<M>>;

  template <typename M, typename R, typename... Args>
  auto Create(Args... args)
    -> std::enable_if_t<
      std::is_same_v<
        decltype(
          std::declval<R>().Create(args...)),
        M*>,
      Model<M>>;

 private:
  template <typename M, typename R, typename... Args>
  Model<M> CreateInternal(M* (R::*mf)(Args...), Args... args);

  std::vector<IRenderer*> renderers_{};
  std::shared_ptr<ICamera> active_camera_{};
};



// implementation
template <typename M, typename... Args>
auto Scene::Create(Args... args)
  -> std::enable_if_t<
    std::is_same_v<
      decltype(
        std::declval<typename M::Renderer>().template Create<M>(args...)),
      M*>,
    Model<M>> {
    return Scene::Create<M, typename M::Renderer>(args...);
}

template <typename M, typename... Args>
auto Scene::Create(Args... args)
  -> std::enable_if_t<
    std::is_same_v<
      decltype(
        std::declval<typename M::Renderer>().Create(args...)),
      M*>,
    Model<M>> {
    return Scene::Create<M, typename M::Renderer>(args...);
}

template <typename M, typename R, typename... Args>
auto Scene::Create(Args... args)
  -> std::enable_if_t<
    std::is_same_v<
      decltype(
        std::declval<R>().template Create<M>(args...)),
      M*>,
    Model<M>> {
  // return CreateInternal<M, R>(&R::template Create<M>, args...);
  using Renderer = R;
  auto& storage = Internal::RendererStorage<Renderer>;
  if (storage.find(this) == storage.end()) {
    storage.insert({this, Renderer{}});
    renderers_.push_back(&storage[this]);
  }
  return Model{storage[this].template Create<M>(args...)};
}

template <typename M, typename R, typename... Args>
auto Scene::Create(Args... args)
  -> std::enable_if_t<
    std::is_same_v<
      decltype(
        std::declval<R>().Create(args...)),
      M*>,
    Model<M>> {
  // return CreateInternal<M, R>(&R::Create, args...);
  using Renderer = R;
  auto& storage = Internal::RendererStorage<Renderer>::GetInstance().storage;
  if (storage.find(this) == storage.end()) {
    storage.insert({this, Renderer{}});
    renderers_.push_back(&storage[this]);
  }
  return Model{storage[this].Create(args...)};
}

// template <typename M, typename R, typename... MFArgs, typename... Args>
// Model<M> Scene::CreateInternal(M* (R::*mf)(MFArgs...), Args... args) {
//   using Renderer = R;
//   auto& storage = Internal::RendererStorage<Renderer>;
//   if (storage.find(this) == storage.end()) {
//     storage.insert({this, Renderer{}});
//     renderers_.push_back(&storage[this]);
//   }
//   return Model{(storage[this].*mf)(args...)};
// }



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_SCENE_H_
