#pragma once

#include <simple3d/core/graphics/primitive.h>

#include <thread>

#include <uWebSockets/App.h>

// FIXME(apachee): It should not be an app, but rather web server, which is used by the actual app
// or whatever it is going to be called in the future.

namespace NSimple3D::NWebServer {

////////////////////////////////////////////////////////////////////////////////

struct IApp
{
    virtual void Run() = 0;

    virtual void UpdateRenderData(std::shared_ptr<NGraphics::TRenderData> renderData) = 0;

    virtual ~IApp() = default;
};

////////////////////////////////////////////////////////////////////////////////

class TApp
    : public IApp
{
public:
    TApp() = default;

    void Run() override;

    //! Thread-Affinity: any.
    void UpdateRenderData(std::shared_ptr<NGraphics::TRenderData> renderData) override;

protected:
    std::shared_ptr<NGraphics::TRenderData> GetRenderData();

    mutable std::mutex RenderDataMutex_;
    std::shared_ptr<NGraphics::TRenderData> RenderData_;

    std::thread RunThread_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebServer
