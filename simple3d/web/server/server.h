#pragma once

#include <simple3d/core/graphics/primitive.h>

#include <thread>

#include <uWebSockets/App.h>

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

// template <typename TWSApp = uWS::App>
// class TApp
//     : public TWSApp
//     , public TAppBase
// {
// public:
//     using TWSApp::TWSApp;

//     TApp();
//     TApp(TWSApp wsApp);

//     TApp RegisterEndpoints();
//     TApp RegisterWebSocket();

//     void ListenAndRunInThread(int port = 8080);

// private:
//     std::thread ListenThread_;
// };

////////////////////////////////////////////////////////////////////////////////

// std::thread RunAppInThread();

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebServer

#define S3D_SERVER_INL_H_
#include "server-inl.h"
#undef S3D_SERVER_INL_H_
