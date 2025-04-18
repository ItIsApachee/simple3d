#include "webgl.h"

#include <simple3d/core/assert.h>

#include <EGL/egl.h>

#include <iostream>

namespace NApachee::NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

namespace NDetail {

////////////////////////////////////////////////////////////////////////////////

#define CASE_STR( value ) case value: return #value; 

const char* EGLGetErrorString(EGLint error)
{
    switch( error )
    {
        CASE_STR(EGL_SUCCESS)
        CASE_STR(EGL_NOT_INITIALIZED)
        CASE_STR(EGL_BAD_ACCESS)
        CASE_STR(EGL_BAD_ALLOC)
        CASE_STR(EGL_BAD_ATTRIBUTE)
        CASE_STR(EGL_BAD_CONTEXT)
        CASE_STR(EGL_BAD_CONFIG)
        CASE_STR(EGL_BAD_CURRENT_SURFACE)
        CASE_STR(EGL_BAD_DISPLAY)
        CASE_STR(EGL_BAD_SURFACE)
        CASE_STR(EGL_BAD_MATCH)
        CASE_STR(EGL_BAD_PARAMETER)
        CASE_STR(EGL_BAD_NATIVE_PIXMAP)
        CASE_STR(EGL_BAD_NATIVE_WINDOW)
        CASE_STR(EGL_CONTEXT_LOST)
        default: return "Unknown";
    }
}

#undef CASE_STR

////////////////////////////////////////////////////////////////////////////////

}  // namespace NDetail

#define S3D_EGL_VERIFY(expr) \
    do { \
        if (!(expr)) { \
            auto message = std::format("EGL error: {}", NDetail::EGLGetErrorString(eglGetError())); \
            ::NApachee::NSimple3D::NDetail::AssertTrapImpl("S3D_EGL_VERIFY", #expr, message); \
        } \
    } while (false)

void Initialize()
{
    // NB(apachee): This is just a proof-of-concept loader and needs lots of polishing.
    // TODO(apachee): More customization?
    // TODO(apachee): Find best configuration?
    // TODO(apachee): Check that chosen config satisfies bare minimum requirements? Like having depth buffer and so on.

    auto display = eglGetDisplay( EGL_DEFAULT_DISPLAY);
    S3D_EGL_VERIFY(display != EGL_NO_DISPLAY);

    S3D_EGL_VERIFY(eglInitialize(display, NULL, NULL));

    EGLConfig config = 0;

    {
        int numConfig = 0;
        S3D_EGL_VERIFY(eglGetConfigs(display, &config, 1, &numConfig));
        S3D_EGL_VERIFY(numConfig == 1);
        S3D_EGL_VERIFY(config);
    }

    constexpr EGLint contextAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_NONE
    };

    S3D_EGL_VERIFY(eglBindAPI(EGL_OPENGL_ES_API));

    auto context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    S3D_EGL_VERIFY(context != EGL_NO_CONTEXT);

    auto surface = eglCreateWindowSurface(display, config, 0, nullptr);

    S3D_EGL_VERIFY(eglMakeCurrent(display, surface, surface, context));
}

#undef S3D_EGL_VERIFY

////////////////////////////////////////////////////////////////////////////////

}  // namespace NApachee::NSimple3D::NGraphics
