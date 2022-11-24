/** \file
 * \brief Definition of Simple3D::Scene
*/

#ifndef SIMPLE3D_GRAPHICS_SCENE_H
#define SIMPLE3D_GRAPHICS_SCENE_H

#include <vector>

#include <simple3d/types.h>
#include <simple3d/graphics/view.h>
#include <simple3d/graphics/renderer.h>

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
    template <typename P, typename... Args>
    P Create(Args... args);
    template <typename P, typename R, typename... Args>
    P Create(Args... args);
private:
    std::vector<IRenderer*> renderers_{};
};



// implementation
template<typename P, typename... Args>
P Scene::Create(Args... args) {
    using Renderer = typename P::Renderer;
    auto& storage = Internal::RendererStorage<Renderer>;
    if (storage.find(this) == storage.end()) {
        storage.insert({this, Renderer{}});
        renderers_.push_back(&storage[this]);
    }
    return storage[this].Create(args...);
}
template <typename P, typename R, typename... Args>
P Scene::Create(Args... args) {
    using Renderer = R;
    auto& storage = Internal::RendererStorage<Renderer>;
    if (storage.find(this) == storage.end()) {
        storage.insert({this, Renderer{}});
        renderers_.push_back(&storage[this]);
    }
    return storage[this].Create(args...);
}



}

#endif