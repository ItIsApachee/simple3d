/** \file
 * \brief Rendering scene to some framebuffer
*/

#ifndef SIMPLE3D_GRAPHICS_RENDERER_SCENE_RENDERER_H
#define SIMPLE3D_GRAPHICS_RENDERER_SCENE_RENDERER_H

#include <simple3d/shader/shader.h>

namespace Simple3D {



/** \class Renderer simple3d/graphics/renderer.h
 * \brief Class that renders scene.
*/
class SceneRenderer {
public:
    SceneRenderer() = default;
    SceneRenderer(const SceneRenderer&) = delete;
    SceneRenderer(SceneRenderer&&) = default;
    SceneRenderer& operator=(const SceneRenderer&) = delete;
    SceneRenderer& operator=(SceneRenderer&&) = default;

    ~SceneRenderer() = default;
private:

};



}

#endif