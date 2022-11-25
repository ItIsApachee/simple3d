/** \file
 * \brief Definition of Simple3D::Scene
*/

#ifndef SIMPLE3D_GRAPHICS_SCENE_H
#define SIMPLE3D_GRAPHICS_SCENE_H

#include <vector>
#include <type_traits>
#include <utility>

#include <simple3d/types.h>
#include <simple3d/graphics/view.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/primitive/primitive.h>

namespace Simple3D {



// definition
/** \class Scene simple3d/graphics/scene.h
 * \brief Container for light sources, and cameras.
*/
class Scene {
public:
    friend class View;
    Scene() = default;
    Scene(const Scene&) = delete;
    Scene(Scene&&) = default;
    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&&) = default;
    ~Scene();

    // TODO: add ability to initialize renderer like Scene::AddRenerer<R>(R&& renderer)
    // TODO: add ability to have multiple instances of renderers of the same type
    // usage example: group objects in separate renderers with different configuration
    // template <typename P, typename... Args>
    // Primitive<P> Create(Args... args);
    template <typename P, typename... Args>
    auto Create(Args... args)
        -> std::enable_if_t<
            std::is_same_v<
                decltype(std::declval<typename P::Renderer>().template Create<P>(args...)),
                P*>,
            Primitive<P>>;
    template <typename P, typename... Args>
    auto Create(Args... args)
        -> std::enable_if_t<
            std::is_same_v<
                decltype(std::declval<typename P::Renderer>().Create(args...)),
                P*>,
            Primitive<P>>;
    template <typename P, typename R, typename... Args>
    auto Create(Args... args)
        -> std::enable_if_t<
            std::is_same_v<
                decltype(std::declval<R>().template Create<P>(args...)),
                P*>,
            Primitive<P>>;
    template <typename P, typename R, typename... Args>
    auto Create(Args... args)
        -> std::enable_if_t<
            std::is_same_v<
                decltype(std::declval<R>().Create(args...)),
                P*>,
            Primitive<P>>;
private:
    template <typename P, typename R, typename... Args>
    Primitive<P> CreateInternal(P* (R::*mf)(Args...), Args... args);

    std::vector<IRenderer*> renderers_{};
};



// implementation
template <typename P, typename... Args>
auto Scene::Create(Args... args)
    -> std::enable_if_t<
        std::is_same_v<
            decltype(std::declval<typename P::Renderer>().template Create<P>(args...)),
            P*>,
        Primitive<P>>
{
    return Scene::Create<P, typename P::Renderer>(args...);
}
template <typename P, typename... Args>
auto Scene::Create(Args... args)
    -> std::enable_if_t<
        std::is_same_v<
            decltype(std::declval<typename P::Renderer>().Create(args...)),
            P*>,
        Primitive<P>>
{
    return Scene::Create<P, typename P::Renderer>(args...);
}
template <typename P, typename R, typename... Args>
auto Scene::Create(Args... args)
    -> std::enable_if_t<
        std::is_same_v<
            decltype(std::declval<R>().template Create<P>(args...)),
            P*>,
        Primitive<P>>
{
    return CreateInternal<P, R>(&R::template Create<P>, args...);
}
template <typename P, typename R, typename... Args>
auto Scene::Create(Args... args)
    -> std::enable_if_t<
        std::is_same_v<
            decltype(std::declval<R>().Create(args...)),
            P*>,
        Primitive<P>>
{
    return CreateInternal<P, R>(&R::Create, args...);
}

template <typename P, typename R, typename... Args>
Primitive<P> Scene::CreateInternal(P* (R::*mf)(Args...), Args... args) {
    using Renderer = R;
    auto& storage = Internal::RendererStorage<Renderer>;
    if (storage.find(this) == storage.end()) {
        storage.insert({this, Renderer{}});
        renderers_.push_back(&storage[this]);
    }
    return Primitive{(storage[this].*mf)(args...)};
}



}

#endif