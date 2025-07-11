#include "renderer_agent.h"

#include "config.h"

#include <GLES3/gl3.h>
#include <emscripten/html5.h>
#include <exception>
#include <simple3d/core/assert.h>
#include <simple3d/core/compiler.h>

#include <simple3d/web/client/lib/graphics/model_shader.h>
#include <simple3d/web/client/lib/graphics/helpers.h>

#include <simple3d/web/client/lib/webgl2/vao.h>
#include <simple3d/web/client/lib/webgl2/vbo.h>
#include <simple3d/web/client/lib/webgl2/webgl.h>

#include <iostream>

namespace NSimple3D::NWebClient {

using namespace NGraphics;
using namespace NWebGL2;

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
        , BackgroundColor_(Config_->InitialBackgroundColor)
    { }

    static TRendererAgentPtr Create(TRendererAgentConfigPtr config)
    {
        auto result =  std::make_shared<TRendererAgent>(std::move(config), TPrivateTag{});
        result->Initialize();
        return result;
    }

    void Start() override
    {
        SetMainLoop(&TRendererAgent::MainLoop, MakeWeak(this));
    }

private:
    // XXX(apachee): The fact than initalization runs at creation time seems fine right now,
    // but there might be some undiscovered problems.
    void Initialize()
    {
        NWebGL2::InitializeOrCrash();

        ModelShader_ = CreateModelShader();
        S3D_UNUSED(ModelShader_->Use());

        InitializeLights();

        InitializeRenderData();

        InitializeVP();
    }

    void InitializeLights()
    {
        // const glm::vec3 white(1.0f);
        // std::vector<std::pair<glm::vec3, glm::vec3>> lightConfigs = {
        //     {glm::vec3(0.0f, -1.0f, 0.0f), white * 0.9f},
        //     {glm::vec3(1.0f, 0.0f, 0.0f), white * 0.8f},
        //     {glm::vec3(0.0f, 0.0f, 1.0f), white * 0.7f},
        //     {glm::vec3(-1.0f, 0.0f, 0.0f), white * 0.55f},
        //     {glm::vec3(0.0f, 0.0f, -1.0f), white * 0.4f}
        // };

        // for (const auto& [dir, baseColor] : lightConfigs) {
        //     DirectionalLights_.insert(std::make_shared<TDirectionalLight>(TDirectionalLight{
        //         dir,
        //         /*diffuse*/ baseColor * 0.5f,
        //         /*specular*/ baseColor * 0.3f
        //     }));
        // }

        // ModelShader_->SetDirectionalLights(DirectionalLights_);

        ModelShader_->SetDirectionalLights({});
        ModelShader_->SetAmbientLight(glm::vec3(1.0f));

        double width;
        double height;
        emscripten_get_element_css_size("#canvas", &width, &height);

        CanvasWidth = static_cast<int>(width);
        CanvasHeight = static_cast<int>(height);
        emscripten_set_canvas_element_size("#canvas", CanvasWidth, CanvasHeight);
    }

    static constexpr int InstanceCount = 2;

    void InitializeRenderData()
    {
        constexpr std::array<TTriangle, InstanceCount> triangles = {
            TTriangle{
                .Model = glm::mat4(1.0f),
                .Vertices = {
                    TVertex{
                        .Pos = glm::vec3(-0.5f, -0.5f, 0.0f),
                    },
                    TVertex{
                        .Pos = glm::vec3(-0.5f, 0.5f, 0.0f),
                    },
                    TVertex{
                        .Pos = glm::vec3(0.5f, -0.5f, 0.0f),
                    },
                },
            },
            TTriangle{
                .Model = glm::mat4(1.0f),
                .Vertices = {
                    TVertex{
                        .Pos = glm::vec3(0.5f, 0.5f, 0.0f),
                    },
                    TVertex{
                        .Pos = glm::vec3(-0.5f, 0.5f, 0.0f),
                    },
                    TVertex{
                        .Pos = glm::vec3(0.5f, -0.5f, 0.0f),
                    },
                },
            },
        };

        Vao_ = TVao::Generate();
        Vbo_ = TVbo(sizeof(TTriangle) * triangles.size(), const_cast<std::byte*>(reinterpret_cast<const std::byte*>(triangles.data())));

        Vao_.Bind();
        Vbo_.Bind();
        TTriangle::BindAttributes();
    }

    void InitializeVP()
    {
        ModelShader_->SetView(glm::mat4(1.0f));
        ModelShader_->SetProj(glm::mat4(1.0f));
    }

    void MainLoop()
    {
        try {
            GuardedMainLoop();
        } catch (const std::exception& ex) {
            std::cerr << std::format("Unhandled exception during render agent main loop iteration: {}", ex.what()) << std::endl;
        } catch (...) {
            std::cerr << std::format("Unhandled exception of unknown type during render agent main loop iteration") << std::endl;
        }
    }

    void GuardedMainLoop()
    {
        // TODO(apachee): Update viewport according to canvas size
        glViewport(0, 0, CanvasWidth, CanvasHeight);

        static int i = 0;
        i++;

        // std::cerr << std::format("i: {}\n", i);

        // glEnable(GL_DEPTH_TEST);
        glClearColor(BackgroundColor_.x, BackgroundColor_.y, BackgroundColor_.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Vao_.Bind();
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, InstanceCount);

        glFlush();

        if (i % 100 == 1) {
            std::cout << i << std::endl;
        }

        NWebGL2::CommitFrame();
    }

    TRendererAgentConfigPtr Config_;
    //! Shader used for rendering.
    //! Created in Initialize rather than at the creation.
    IShaderPtr ModelShader_;
    std::unordered_set<std::shared_ptr<TDirectionalLight>> DirectionalLights_;
    // const bool MutableLights_ = false;
    // bool AreLightsSet_ = false;
    const glm::vec3 BackgroundColor_;

    // Render data.
    TVao Vao_;
    TVbo Vbo_;

    int CanvasWidth;
    int CanvasHeight;
};

////////////////////////////////////////////////////////////////////////////////

IRendererAgentPtr CreateRendererAgent(TRendererAgentConfigPtr config)
{
    return TRendererAgent::Create(std::move(config));
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebClient
