#include "bootstrap.h"

#include "config.h"
#include "renderer_agent.h"

namespace NSimple3D::NWebClient {

////////////////////////////////////////////////////////////////////////////////

S3D_DECLARE_REFCOUNTED_CLASS(TBootstrap);

class TBootstrap
    : public IBootstrap
{
private:
    struct TPrivateTag
    { };

public:
    TBootstrap(
        TBootstrapConfigPtr config, TPrivateTag)
        : Config_(std::move(config))
    { }

    static TBootstrapPtr Create(TBootstrapConfigPtr config)
    {
        return std::make_shared<TBootstrap>(std::move(config), TPrivateTag{});
    }

    void Initialize()
    {
        RendererAgent = CreateRendererAgent(Config_->RendererAgent);
    }

    void Start() override
    {
        RendererAgent->Start();
    }

private:
    TBootstrapConfigPtr Config_;

    IRendererAgentPtr RendererAgent;
};

////////////////////////////////////////////////////////////////////////////////

IBootstrapPtr CreateBootstrap(TBootstrapConfigPtr config)
{
    auto bootstrap = TBootstrap::Create(std::move(config));
    bootstrap->Initialize();
    return bootstrap;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebClient
