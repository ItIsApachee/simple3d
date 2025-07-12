#pragma once

#include <condition_variable>
#include <simple3d/core/types.h>
#include <simple3d/core/shared_ptr.h>

#include <emscripten/html5.h>
#include <emscripten/websocket.h>

#include <deque>
#include <memory>
#include <span>
#include <string>
#include <vector>
#include <thread>

// TODO(apachee): Works for now, but maybe
// there is a better way to implement IO.

namespace NSimple3D {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct IIoHandler
{ };

template <typename R, typename... Args>
struct IIoHandler<R(Args...)>
{
    virtual R Handle(Args... args) = 0;

    virtual ~IIoHandler() = default;
};

////////////////////////////////////////////////////////////////////////////////

using IKeyHandler = IIoHandler<bool(int, const EmscriptenKeyboardEvent*)>;

////////////////////////////////////////////////////////////////////////////////

bool KeyCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void*);

bool CheckKey(const std::string& s);

////////////////////////////////////////////////////////////////////////////////

template <typename T = i64>
struct TCursorPos
{
    T X = 0;
    T Y = 0;
};

using IMouseMoveHandler = IIoHandler<void(int, const EmscriptenMouseEvent*)>;

bool MouseMoveCallback(int eventType, const EmscriptenMouseEvent*, void*);

void SubscribeOnMouseMove(IMouseMoveHandler* handler);
void UnsubscribeOnMouseMove(IMouseMoveHandler* handler);

bool IsPointerLockActive();

////////////////////////////////////////////////////////////////////////////////

S3D_DECLARE_REFCOUNTED_CLASS(TWebSocket);

enum struct EWebSocketState : int
{
    Connecting = 0,
    Open = 1,
    Closing = 2,
    Closed = 3,
};

class TWebSocket
    : public std::enable_shared_from_this<TWebSocket>
{
private:
    struct TPrivateTag
    { };
public:
    static TWebSocketPtr Create(std::string url, std::string protocols, std::thread& wsThreadStd);

    TWebSocket(EMSCRIPTEN_WEBSOCKET_T websocketHandle, TPrivateTag);

    ~TWebSocket();

    void SendBinary(std::span<const std::byte> binaryData);

    std::vector<std::byte> ReadExactlyN(i64 n);

private:
    static bool OnOpen(int /*eventType*/, const EmscriptenWebSocketOpenEvent* websocketEvent __attribute__((nonnull)), void*);
    static bool OnMessage(int /*eventType*/, const EmscriptenWebSocketMessageEvent* websocketEvent __attribute__((nonnull)), void*);
    static bool OnError(int /*eventType*/, const EmscriptenWebSocketErrorEvent* websocketEvent __attribute__((nonnull)), void*);
    static bool OnClose(int /*eventType*/, const EmscriptenWebSocketCloseEvent* websocketEvent __attribute__((nonnull)), void*);

    void Initialize(std::thread& wsThreadStd);

    void OnMessageImpl(std::span<const std::byte> message);

    EMSCRIPTEN_WEBSOCKET_T WebSocketHandle_;

    std::deque<std::byte> Buffer_;
    mutable std::mutex BufferMutex_;
    mutable std::condition_variable BufferCondVar_;

    std::atomic<EWebSocketState> State_ = EWebSocketState::Connecting;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D
