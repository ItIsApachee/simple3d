#include "renderer.h"

#include <simple3d/core/assert.h>

#include <simple3d/web/client/lib/webgl2/webgl.h>

#include <simple3d/web/client/lib/helpers.h>

#include <GLES3/gl3.h>

namespace NSimple3D::NGLib {

namespace {

////////////////////////////////////////////////////////////////////////////////

class TRenderer
{
public:
    TRenderer()
    {

    }

    void Render()
    {
        // TODO(apachee): Update viewport according to canvas size
        glViewport(0, 0, 800, 600);

        static int i = 0;
        i++;

        // std::cerr << std::format("i: {}\n", i);

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat v = (sin(i / 100.0f) + 1) / 2.0f;
        // std::cerr << "v: " << v << std::endl;
        glClearColor(v, v, v, 1.0f);

        glFlush();

        NWebGL2::CommitFrame();
    }
};

////////////////////////////////////////////////////////////////////////////////

thread_local TRenderer* Renderer = nullptr;

////////////////////////////////////////////////////////////////////////////////

}  // namespace

////////////////////////////////////////////////////////////////////////////////

void Start()
{
    NWebGL2::InitializeOrCrash();

    NGLib::SetMainLoop();
}

////////////////////////////////////////////////////////////////////////////////

void SetMainLoop()
{
    if (!Renderer) {
        Renderer = new TRenderer();
    }

    ::NSimple3D::SetMainLoop([] (TRenderer* renderer) {
        renderer->Render();
    }, Renderer);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NGLib