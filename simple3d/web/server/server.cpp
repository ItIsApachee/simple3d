#include "server.h"

namespace NSimple3D::NWebServer {

////////////////////////////////////////////////////////////////////////////////

std::thread RunAppInThread()
{
    return std::thread([] {
        auto app = TApp()
            .RegisterEndpoints()
            .RegisterWebSocket()
            .ListenAndRun();
    });
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebServer
