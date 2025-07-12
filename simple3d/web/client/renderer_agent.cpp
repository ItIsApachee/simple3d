#include "renderer_agent.h"

#include "render_data_fetcher.h"

#include "config.h"

#include <simple3d/web/client/lib/graphics/config.h>
#include <simple3d/web/client/lib/graphics/fps_camera.h>
#include <simple3d/web/client/lib/graphics/helpers.h>
#include <simple3d/web/client/lib/graphics/model_shader.h>

#include <simple3d/web/client/lib/webgl2/vao.h>
#include <simple3d/web/client/lib/webgl2/vbo.h>
#include <simple3d/web/client/lib/webgl2/webgl.h>

#include <simple3d/core/assert.h>
#include <simple3d/core/compiler.h>

#include <GLES3/gl3.h>

#include <emscripten/html5.h>

#include <chrono>
#include <exception>
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
        , RenderDataFetcher_(CreateRenderDataFetcher())
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
    void Initialize()
    {
        NWebGL2::InitializeOrCrash();

        ModelShader_ = CreateModelShader();
        S3D_UNUSED(ModelShader_->Use());

        InitializeLights();

        InitializeRenderData();
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

        FpsCamera_ = TFpsCamera::Create(TFpsCameraConfig{
            .AspectRatio = static_cast<float>(CanvasWidth) / static_cast<float>(CanvasHeight),
        });
    }

    void InitializeRenderData()
    {
        Vao_ = TVao::Generate();
        Vbo_ = TVbo(4096, nullptr);

        Vao_.Bind();
        Vbo_.Bind();
        BindTriangleAttributes();
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
        auto renderData = RenderDataFetcher_->Fetch();
        Vbo_.SubData(0, sizeof(TTriangle) * renderData->size(), reinterpret_cast<std::byte*>(renderData->data()));

        auto now = std::chrono::steady_clock::now();
        auto delta = now - LastFrame_;
        LastFrame_ = now;
        FpsCamera_.Update(std::chrono::duration_cast<std::chrono::milliseconds>(delta));

        // TODO(apachee): Update viewport according to canvas size
        glViewport(0, 0, CanvasWidth, CanvasHeight);

        ModelShader_->SetView(FpsCamera_.GetView());
        ModelShader_->SetProj(FpsCamera_.GetProj());

        // static int i = 0;
        // i++;

        // std::cerr << std::format("i: {}\n", i);

        // glEnable(GL_DEPTH_TEST);
        glClearColor(BackgroundColor_.x, BackgroundColor_.y, BackgroundColor_.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Vao_.Bind();
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, renderData->size());

        glFlush();

        // if (i % 400 == 1) {
        //     std::cout << i << std::endl;
        // }

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

    TFpsCamera FpsCamera_;
    std::chrono::time_point<std::chrono::steady_clock> LastFrame_ = std::chrono::steady_clock::now();

    IRenderDataFetcherPtr RenderDataFetcher_;
};

////////////////////////////////////////////////////////////////////////////////

IRendererAgentPtr CreateRendererAgent(TRendererAgentConfigPtr config)
{
    return TRendererAgent::Create(std::move(config));
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebClient
