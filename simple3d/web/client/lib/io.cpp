#include "io.h"

#include <simple3d/core/assert.h>
#include <simple3d/core/error.h>
#include <simple3d/core/shared_ptr.h>

#include <emscripten/html5.h>
#include <emscripten/websocket.h>

#include <format>
#include <iostream>
#include <mutex>
#include <memory>
#include <thread>
#include <unordered_set>

namespace NSimple3D {

////////////////////////////////////////////////////////////////////////////////

static std::unordered_map<std::string, bool>& KeyToState = *(new std::unordered_map<std::string, bool>());
static std::mutex KeyToStateMutex;

bool KeyCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void*)
{
    bool isDown = eventType == EMSCRIPTEN_EVENT_KEYDOWN;
    bool isUp = eventType == EMSCRIPTEN_EVENT_KEYUP;
    S3D_VERIFY((isDown ^ isUp) != 0);

    auto keyCode = std::string(static_cast<const char*>(keyEvent->code));
    {
        std::lock_guard<std::mutex> guard(KeyToStateMutex);
        KeyToState[keyCode] = isDown;
    }

    return true;
}

bool CheckKey(const std::string& s)
{
    std::lock_guard<std::mutex> guard(KeyToStateMutex);
    return KeyToState[s];
}

////////////////////////////////////////////////////////////////////////////////

static std::unordered_set<IMouseMoveHandler*>& MouseMoveSubscribers = *(new std::unordered_set<IMouseMoveHandler*>());

////////////////////////////////////////////////////////////////////////////////

bool MouseMoveCallback(int eventType, const EmscriptenMouseEvent* event, void*)
{
    S3D_VERIFY(eventType == EMSCRIPTEN_EVENT_MOUSEMOVE);

    for (auto handler : MouseMoveSubscribers) {
        handler->Handle(eventType, event);
    }

    return true;
}

void SubscribeOnMouseMove(IMouseMoveHandler* handler)
{
    MouseMoveSubscribers.insert(handler);
}

void UnsubscribeOnMouseMove(IMouseMoveHandler* handler)
{
    MouseMoveSubscribers.erase(handler);
}

bool IsPointerLockActive()
{
    EmscriptenPointerlockChangeEvent status;
    emscripten_get_pointerlock_status(&status);
    return status.isActive;
}

////////////////////////////////////////////////////////////////////////////////

static auto& WebSockets = *(new std::unordered_map<EMSCRIPTEN_WEBSOCKET_T, std::weak_ptr<TWebSocket>>());

////////////////////////////////////////////////////////////////////////////////

bool TWebSocket::OnOpen(int /*eventType*/, const EmscriptenWebSocketOpenEvent* websocketEvent __attribute__((nonnull)), void*)
{
    auto weakWS = WebSockets.at(websocketEvent->socket);
    auto ws = weakWS.lock();
    S3D_VERIFY(ws);
    S3D_VERIFY(ws->State_.load() == EWebSocketState::Connecting);

    ws->State_.store(EWebSocketState::Open);

    std::cout << std::format("websocket open") << std::endl;

    return true;
}

bool TWebSocket::OnMessage(int /*eventType*/, const EmscriptenWebSocketMessageEvent* websocketEvent __attribute__((nonnull)), void*)
{
    auto weakWS = WebSockets.at(websocketEvent->socket);
    auto ws = weakWS.lock();
    S3D_VERIFY(ws);

    S3D_VERIFY(!websocketEvent->isText);
    std::span<const std::byte> message(
        reinterpret_cast<const std::byte*>(websocketEvent->data),
        reinterpret_cast<const std::byte*>(websocketEvent->data + websocketEvent->numBytes));

    ws->OnMessageImpl(message);

    return true;
}

bool TWebSocket::OnError(int /*eventType*/, const EmscriptenWebSocketErrorEvent* /*websocketEvent*/ __attribute__((nonnull)), void*)
{
    std::cout << std::format("websocket error") << std::endl;
    return true;
}

bool TWebSocket::OnClose(int /*eventType*/, const EmscriptenWebSocketCloseEvent* websocketEvent __attribute__((nonnull)), void*)
{
    auto weakWS = WebSockets.at(websocketEvent->socket);
    auto ws = weakWS.lock();
    S3D_VERIFY(ws);

    ws->State_.store(EWebSocketState::Closed);

    std::cout << std::format("websocket closing, reason: {}", static_cast<const char*>(websocketEvent->reason)) << std::endl;
    S3D_VERIFY(websocketEvent->wasClean);
    return true;
}

////////////////////////////////////////////////////////////////////////////////

TWebSocketPtr TWebSocket::Create(std::string url, std::string protocols, std::thread& wsThreadStd)
{
    EmscriptenWebSocketCreateAttributes attributes{
        .url = url.c_str(),
        .protocols = protocols.c_str(),
        .createOnMainThread = false,
    };

    auto websocketHandle = emscripten_websocket_new(&attributes);
    S3D_VERIFY(websocketHandle > 0);

    auto websocket =  std::make_shared<TWebSocket>(websocketHandle, TPrivateTag{});
    websocket->Initialize(wsThreadStd);
    return websocket;
}

TWebSocket::TWebSocket(EMSCRIPTEN_WEBSOCKET_T webSocketHandle, TPrivateTag)
    : WebSocketHandle_(webSocketHandle)
{ }

void TWebSocket::Initialize(std::thread& wsThreadStd)
{
    auto wsThread = wsThreadStd.native_handle();
    // WSThreadId_ = wsThreadStd.get_id();
    WebSockets[WebSocketHandle_] = MakeStrong(this);

    emscripten_websocket_set_onopen_callback_on_thread(WebSocketHandle_, 0, &TWebSocket::OnOpen, wsThread);
    emscripten_websocket_set_onmessage_callback_on_thread(WebSocketHandle_, 0, &TWebSocket::OnMessage, wsThread);
    emscripten_websocket_set_onerror_callback_on_thread(WebSocketHandle_, 0, &TWebSocket::OnError, wsThread);
    emscripten_websocket_set_onclose_callback_on_thread(WebSocketHandle_, 0, &TWebSocket::OnClose, wsThread);

    unsigned short readyState;
    emscripten_websocket_get_ready_state(WebSocketHandle_, &readyState);
    if (readyState == static_cast<int>(EWebSocketState::Open)) {
        return;
    }

    while (static_cast<int>(State_.load()) < static_cast<int>(EWebSocketState::Open)) {
        std::cout << "waiting on open websocket" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::this_thread::yield();
    }
}

TWebSocket::~TWebSocket()
{
    WebSockets.erase(WebSocketHandle_);
}

void TWebSocket::SendBinary(std::span<const std::byte> binaryData)
{
    emscripten_websocket_send_binary(WebSocketHandle_, reinterpret_cast<void*>(const_cast<std::byte*>(binaryData.data())), binaryData.size_bytes());
}

std::vector<std::byte> TWebSocket::ReadExactlyN(i64 n)
{
    S3D_VERIFY(n >= 0);
    if (n == 0) {
        return {};
    }

    std::unique_lock guard(BufferMutex_);
    // XXX(apachee): Timeout?
    BufferCondVar_.wait(guard, [&] {
        return Buffer_.size() >= n || State_ != EWebSocketState::Open;
    });

    if (Buffer_.size() < n) {
        S3D_THROW("WebSocket closed during read");
    }

    std::vector<std::byte> result(Buffer_.begin(), Buffer_.begin() + n);
    // S3D_VERIFY(BufferMutex_.try_lock());
    Buffer_.erase(Buffer_.begin(), Buffer_.begin() + n);
    return result;
}

void TWebSocket::OnMessageImpl(std::span<const std::byte> message)
{
    {
        std::lock_guard guard(BufferMutex_);
        Buffer_.insert(Buffer_.end(), message.begin(), message.end());
    }
    BufferCondVar_.notify_one();
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D
