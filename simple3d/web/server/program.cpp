#include "server.h"

#include <uWebSockets/App.h>

#include <fstream>
#include <sstream>

namespace NSimple3D::NWebServer {

////////////////////////////////////////////////////////////////////////////////

int Main()
{
    auto app = TApp<uWS::App>()
        .RegisterEndpoints()
        .RegisterWebSocket()
        .ListenAndRun();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebServer

int main() {
    return ::NSimple3D::NWebServer::Main();
}
