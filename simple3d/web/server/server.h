#pragma once

#include <thread>

#include <uWebSockets/App.h>

namespace NSimple3D::NWebServer {

////////////////////////////////////////////////////////////////////////////////

struct IApp
{
   

    virtual ~IApp() = default;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TWSApp = uWS::App>
struct TApp
    : public TWSApp
{
    using TWSApp::TWSApp;

    TApp();
    TApp(TWSApp wsApp);

    TApp RegisterEndpoints();
    TApp RegisterWebSocket();

    TApp ListenAndRun(int port = 8080);
};

////////////////////////////////////////////////////////////////////////////////

std::thread RunAppInThread();

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebServer

#define S3D_SERVER_INL_H_
#include "server-inl.h"
#undef S3D_SERVER_INL_H_
