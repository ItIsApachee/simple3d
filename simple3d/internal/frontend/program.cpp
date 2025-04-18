#include <simple3d/core/assert.h>

#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/emscripten.h>

#include <EGL/egl.h>

#include <GLES3/gl3.h>

#include <iostream>
#include <format>
#include <cassert>

#define CASE_STR( value ) case value: return #value; 
const char* eglGetErrorString( EGLint error )
{
    switch( error )
    {
    CASE_STR( EGL_SUCCESS             )
    CASE_STR( EGL_NOT_INITIALIZED     )
    CASE_STR( EGL_BAD_ACCESS          )
    CASE_STR( EGL_BAD_ALLOC           )
    CASE_STR( EGL_BAD_ATTRIBUTE       )
    CASE_STR( EGL_BAD_CONTEXT         )
    CASE_STR( EGL_BAD_CONFIG          )
    CASE_STR( EGL_BAD_CURRENT_SURFACE )
    CASE_STR( EGL_BAD_DISPLAY         )
    CASE_STR( EGL_BAD_SURFACE         )
    CASE_STR( EGL_BAD_MATCH           )
    CASE_STR( EGL_BAD_PARAMETER       )
    CASE_STR( EGL_BAD_NATIVE_PIXMAP   )
    CASE_STR( EGL_BAD_NATIVE_WINDOW   )
    CASE_STR( EGL_CONTEXT_LOST        )
    default: return "Unknown";
    }
}
#undef CASE_STR


void main_loop()
{
    static int i = 0;
    i++;

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat v = (sin(i / 15.0f) + 1) / 2.0f;
    std::cerr << "v: " << v << std::endl;
    glClearColor(v, v, v, 1.0f);

    glFlush();
}

int main()
{

#define S3D_EGL_VERIFY(expr) \
    do { \
        if (!(expr)) { \
            std::cerr << #expr << " is false, egl error: " << eglGetErrorString(eglGetError()) << std::endl; \
            S3D_VERIFY(expr);\
        } \
    } while (false)

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

    glViewport(0, 0, 800, 600);

// #ifndef NDEBUG
// #endif

    // emscripten_set_main_loop();
    emscripten_set_main_loop(&main_loop, 0, true);


    // assert(eglInitialize(display, nullptr, nullptr));

    // EGLConfig* configs{};

}