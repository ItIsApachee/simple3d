#include "interop.h"

#include <emscripten/bind.h>

#include <condition_variable>
#include <iostream>

namespace NSimple3D::NWebClient {

////////////////////////////////////////////////////////////////////////////////

namespace {

////////////////////////////////////////////////////////////////////////////////

std::mutex InitWebSocketMutex;
std::condition_variable InitWebSocketCondVar;
std::optional<std::string> WebSocketUrl;

////////////////////////////////////////////////////////////////////////////////

}  // namespace

////////////////////////////////////////////////////////////////////////////////

void Simple3DInitWebSocket(std::string url)
{
    std::cout << std::format("Simple3DInitWebSocket called with url = {}", url) << std::endl;

    {
        std::lock_guard guard(InitWebSocketMutex);
        WebSocketUrl = url;
    }
    InitWebSocketCondVar.notify_all();
}

////////////////////////////////////////////////////////////////////////////////

EMSCRIPTEN_BINDINGS(Simple3DInterop)
{
    emscripten::function("Simple3DInitWebSocket", &Simple3DInitWebSocket);
}

////////////////////////////////////////////////////////////////////////////////

std::string GetInitWebSocketOptions()
{
    std::unique_lock guard(InitWebSocketMutex);
    InitWebSocketCondVar.wait(guard, [] {
        return WebSocketUrl.has_value();
    });

    return *WebSocketUrl;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebClient
