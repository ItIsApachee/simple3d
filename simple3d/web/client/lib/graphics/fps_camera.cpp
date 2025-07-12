#include "fps_camera.h"

#include <emscripten/html5.h>
#include <mutex>
#include <simple3d/core/assert.h>
#include <simple3d/web/client/lib/io.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>
#include <format>

namespace NSimple3D::NGraphics {

////////////////////////////////////////////////////////////////////////////////

// FIXME(apachee): Fix mess with re-creation.
class TFpsCamera::TMouseMoveHandler
    : public IMouseMoveHandler
{
public:
    TMouseMoveHandler(TFpsCamera* fpsCamera)
        : FpsCamera_(fpsCamera)
    { }

    void Initialize()
    {
        SubscribeOnMouseMove(this);
    }

    ~TMouseMoveHandler()
    {
        UnsubscribeOnMouseMove(this);
    }

    void Handle(int eventType, const EmscriptenMouseEvent* event) override
    {
        // std::cout << "fps cam mouse move handler called: " << FpsCamera_ << std::endl;

        S3D_VERIFY(eventType == EMSCRIPTEN_EVENT_MOUSEMOVE);

        if (!FpsCamera_->Focused.load()) {
            return;
        }

        {
            std::lock_guard<std::mutex> guard(FpsCamera_->CursorPosMutex);
            auto& [x, y] = FpsCamera_->AccumulatedCursorPos;
            x += event->movementX;
            y += event->movementY;
        }
    }

private:
    TFpsCamera* FpsCamera_;
};

////////////////////////////////////////////////////////////////////////////////

// class TFpsCamera::TKeyHandler
//     : public IKeyHandler
// {
// public:
//     TKeyHandler(TFpsCamera* fpsCamera)
//         : FpsCamera_(fpsCamera)
//     { }

//     bool Handle(int eventType, const EmscriptenKeyboardEvent* keyEvent) override
//     {

//     }

// private:
//     TFpsCamera* FpsCamera_;
// };

TFpsCamera TFpsCamera::Create(TFpsCameraConfig config)
{
    auto result = TFpsCamera(std::move(config), TPrivateTag{});
    result.Initialize();
    return result;
}

TFpsCamera::TFpsCamera() = default;

TFpsCamera::TFpsCamera(TFpsCameraConfig config, TPrivateTag)
    : Config_(config)
    , State_()
{
    State_.AspectRatio = config.AspectRatio;
    State_.Fov = config.Fov;
}

void TFpsCamera::Initialize()
{
    MouseMoveHandler_ = std::make_unique<TMouseMoveHandler>(this);
    MouseMoveHandler_->Initialize();
}

TFpsCamera::TFpsCamera(TFpsCamera&& other)
{
    Config_ = std::move(other.Config_);
    State_ = std::move(other.State_);
    Focused = std::move(other.Focused.load());
    MouseMoveHandler_ = std::make_unique<TMouseMoveHandler>(this);
    MouseMoveHandler_->Initialize();
    {
        std::lock_guard<std::mutex> guardOther(other.CursorPosMutex);
        std::lock_guard<std::mutex> guard(CursorPosMutex);
        AccumulatedCursorPos = std::move(other.AccumulatedCursorPos);
        LastCursorPos = std::move(other.LastCursorPos);
    }
}

TFpsCamera& TFpsCamera::operator=(TFpsCamera&& other)
{
    Config_ = std::move(other.Config_);
    State_ = std::move(other.State_);
    Focused = std::move(other.Focused.load());
    MouseMoveHandler_ = std::make_unique<TMouseMoveHandler>(this);
    MouseMoveHandler_->Initialize();
    {
        std::lock_guard<std::mutex> guardOther(other.CursorPosMutex);
        std::lock_guard<std::mutex> guard(CursorPosMutex);
        AccumulatedCursorPos = std::move(other.AccumulatedCursorPos);
        LastCursorPos = std::move(other.LastCursorPos);
    }
    return *this;
}

TFpsCamera::~TFpsCamera() = default;

glm::mat4 TFpsCamera::GetView() const
{
    return State_.GetView();
}

glm::mat4 TFpsCamera::GetProj() const
{
    return State_.GetProj();
}

glm::vec3 TFpsCamera::GetViewPos() const
{
    return State_.GetViewPos();
}

void TFpsCamera::Update(const std::chrono::milliseconds& delta)
{
    constexpr float kMillisPerSecond = 1000.0F;
    float mult = Config_.MovementSpeed * delta.count() / kMillisPerSecond;

    auto keyStates = GetKeyStates();
    if (keyStates.Enter && !Focused.load()) {
        Focused.store(true);
        emscripten_request_pointerlock("#canvas", true);
    }
    if (keyStates.Escape && Focused.load()) {
        Focused.store(false);
        emscripten_exit_pointerlock();
    }
    if (!IsPointerLockActive()) {
        Focused.store(false);
    }
    if (!Focused) {
        return;
    }

    glm::vec3 camFront{-glm::sin(-State_.Yaw), 0.0F,
                        -glm::cos(-State_.Yaw)};
    float frontMult = mult * (keyStates.Forward - keyStates.Backward);
    State_.Pos += frontMult * camFront;

    glm::vec3 camRight{glm::cos(-State_.Yaw), 0.0F,
                        -glm::sin(-State_.Yaw)};
    float rightMult = mult * (keyStates.Right - keyStates.Left);
    State_.Pos += rightMult * camRight;

    glm::vec3 camUp{0.0F, 1.0F, 0.0F};
    float upMult = mult * (keyStates.Up - keyStates.Down);
    State_.Pos += upMult * camUp;

    TCursorPos<float> Diff{};
    {
       std::lock_guard<std::mutex> guard(CursorPosMutex);
    //    std::cout << std::format("mouse acc {} {}", AccumulatedCursorPos.X, AccumulatedCursorPos.Y) << std::endl;
       Diff.X = AccumulatedCursorPos.X - LastCursorPos.X;
       Diff.Y = AccumulatedCursorPos.Y - LastCursorPos.Y;
       LastCursorPos = AccumulatedCursorPos;
    }


    // FIXME(apachee): Disabled for hackathon.
    const double pixels_to_radians = glm::radians(0.25f);

    Diff.X *= pixels_to_radians;
    using yaw_t = decltype(State_.Yaw);
    State_.Yaw += Config_.MouseSensitivity * Diff.X;
    State_.Yaw = std::fmod(State_.Yaw, glm::two_pi<yaw_t>());

    Diff.Y *= pixels_to_radians;
    using pitch_t = decltype(State_.Pitch);
    State_.Pitch += Config_.MouseSensitivity * Diff.Y;
    State_.Pitch = std::clamp<pitch_t>(
        State_.Pitch, -glm::half_pi<pitch_t>(), glm::half_pi<pitch_t>());

    // std::cout << "Fps cam update this: " << this << std::endl;
    // std::cout << std::format("pos {} {} {}", State_.Pos.x, State_.Pos.y, State_.Pos.z) << std::endl;
}

TFpsCamera::TKeyStates TFpsCamera::GetKeyStates()
{
    TKeyStates keyStates{};

    keyStates.Forward = CheckKey("KeyW");
    keyStates.Backward = CheckKey("KeyS");
    keyStates.Left = CheckKey("KeyA");
    keyStates.Right = CheckKey("KeyD");
    keyStates.Up = CheckKey("Space");
    keyStates.Down = CheckKey("ShiftLeft");
    keyStates.Escape = CheckKey("Escape");
    keyStates.Enter = CheckKey("Enter");

    return keyStates;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NGraphics
