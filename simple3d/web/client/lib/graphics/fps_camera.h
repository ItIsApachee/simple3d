#pragma once

#include "config.h"
#include "camera.h"

#include <simple3d/web/client/lib/io.h>

#include <chrono>

namespace NSimple3D::NGraphics {

////////////////////////////////////////////////////////////////////////////////

class TFpsCamera
{
private:
    struct TPrivateTag
    { };
public:
    static TFpsCamera Create(TFpsCameraConfig config);

    TFpsCamera();
    explicit TFpsCamera(TFpsCameraConfig config, TPrivateTag);

    TFpsCamera(TFpsCamera&&);

    TFpsCamera& operator=(TFpsCamera&&);

    ~TFpsCamera();

    void Initialize();

    // TCameraState3D GetCameraState();

    glm::mat4 GetView() const;
    glm::mat4 GetProj() const;
    glm::vec3 GetViewPos() const;

    void Update(const std::chrono::milliseconds& delta);

private:
    class TMouseMoveHandler;

    // class TKeyHandler;
    struct TKeyStates {
        char Forward{0};
        char Backward{0};
        char Left{0};
        char Right{0};
        char Up{0};
        char Down{0};
        bool Enter{false};
        bool Escape{false};
    };

    TKeyStates GetKeyStates();

    TFpsCameraConfig Config_;
    TCameraState3D State_{};
    // bool Focused = false;
    std::atomic<bool> Focused = false;
    // std::unique_ptr<TKeyHandler> KeyHandler_;

    std::unique_ptr<TMouseMoveHandler> MouseMoveHandler_;

    mutable std::mutex CursorPosMutex;
    TCursorPos<i64> AccumulatedCursorPos{};
    TCursorPos<i64> LastCursorPos{};
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NGraphics
