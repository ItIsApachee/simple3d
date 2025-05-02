#include "renderer_agent.h"

#include "config.h"

#include <simple3d/core/assert.h>

#include <simple3d/web/client/lib/webgl2/webgl.h>

#include <simple3d/web/client/lib/graphics/helpers.h>

#include <GLES3/gl3.h>

namespace NSimple3D::NWebClient {

using namespace NGraphics;

////////////////////////////////////////////////////////////////////////////////

S3D_DECLARE_REFCOUNTED_CLASS(TRendererAgent);

class TRendererAgent
    : public IRendererAgent
{
private:
    struct TPrivateTag
    { };

public:
    TRendererAgent(
        TRendererAgentConfigPtr config, TPrivateTag)
        : Config_(std::move(config))
    { }

    static TRendererAgentPtr Create(TRendererAgentConfigPtr config)
    {
        return std::make_shared<TRendererAgent>(std::move(config), TPrivateTag{});
    }

    void Start() override
    {
        NWebGL2::InitializeOrCrash();

        SetMainLoop(&TRendererAgent::MainLoop, MakeWeak(this));
    }

private:

    void MainLoop()
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

    TRendererAgentConfigPtr Config_;
};

////////////////////////////////////////////////////////////////////////////////

IRendererAgentPtr CreateRendererAgent(TRendererAgentConfigPtr config)
{
    return TRendererAgent::Create(std::move(config));
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebClient
