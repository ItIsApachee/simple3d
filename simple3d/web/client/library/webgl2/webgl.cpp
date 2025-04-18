#include "webgl.h"

#include <emscripten/em_types.h>
#include <simple3d/core/assert.h>

#include <emscripten/html5_webgl.h>

#include <iostream>
#include <mutex>
#include <optional>

namespace NApachee::NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

namespace {

////////////////////////////////////////////////////////////////////////////////

struct TEglState
{
    static TEglState& Get() {
        static TEglState v{};
        return v;
    }

    template <typename M>
    void CheckInvariants(const std::lock_guard<M>& /*lock*/) const
    {
        if (Initialized) {
            S3D_VERIFY(Context);
            S3D_VERIFY(*Context != 0);
        } else {
            S3D_VERIFY(!Context);
        }
    }

    mutable std::mutex Mutex;

    bool Initialized = false;
    std::optional<EMSCRIPTEN_WEBGL_CONTEXT_HANDLE> Context;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace

void InitializeOrCrash()
{
    auto& eglState = TEglState::Get();
    std::lock_guard lock(eglState.Mutex);

    S3D_VERIFY(!eglState.Initialized);

    // NB(apachee): This is just a proof-of-concept loader and needs lots of polishing.
    // TODO(apachee): More customization?
    // TODO(apachee): Find best configuration?
    // TODO(apachee): Check that chosen config satisfies bare minimum requirements? Like having depth buffer and so on.

    EmscriptenWebGLContextAttributes webGLContextAttributes;
    webGLContextAttributes.explicitSwapControl = true;
    emscripten_webgl_init_context_attributes(&webGLContextAttributes);
    webGLContextAttributes.majorVersion = 2;

    auto context = emscripten_webgl_create_context("#canvas", &webGLContextAttributes);
    S3D_VERIFY(context != 0);

    if (auto error = emscripten_webgl_make_context_current(context); error != EMSCRIPTEN_RESULT_SUCCESS) {
        auto errorMessage = std::format("failed to active WebGL context with error code: {}", error);
        S3D_ALERT(errorMessage);
    }

    eglState.Initialized = true;
    eglState.Context = context;

    eglState.CheckInvariants(lock);
}

void Terminate()
{
    auto& eglState = TEglState::Get();
    std::lock_guard lock(eglState.Mutex);

    S3D_VERIFY(eglState.Initialized);

    if (auto error = emscripten_webgl_destroy_context(*eglState.Context); error != EMSCRIPTEN_RESULT_SUCCESS) {
        auto errorMessage = std::format("failed to destroy WebGL context with error code: {}", error);
        S3D_ALERT(errorMessage);
    }
    eglState.Context.reset();
    
    eglState.Initialized = false;

    eglState.CheckInvariants(lock);
}

////////////////////////////////////////////////////////////////////////////////

void CommitFrame()
{
    emscripten_webgl_commit_frame();
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NApachee::NSimple3D::NGraphics
