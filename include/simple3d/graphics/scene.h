/** \file
 * \brief Definition of Simple3D::Scene
*/

#ifndef SIMPLE3D_GRAPHICS_SCENE_H
#define SIMPLE3D_GRAPHICS_SCENE_H

#include <thread>
#include <memory>
#include <atomic>

#include <simple3d/types.h>
#include <simple3d/context/context.h>

namespace Simple3D {



class Scene : public std::enable_shared_from_this<Scene> {
public:
    friend void render_scene(std::shared_ptr<Scene> scene);

    static std::shared_ptr<Scene> Create();

    void SetContext(std::shared_ptr<Window> window);
    void ClearContext();

    ~Scene() = default;
private:
    Scene();
    
    std::thread render_thread_{};
    std::atomic_flag is_running_{};

    std::shared_ptr<Window> window_{};
};



}

#endif