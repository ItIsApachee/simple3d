#include <simple3d/internal/frontend/graphics/webgl2/webgl.h>

#include <simple3d/core/assert.h>

#include <emscripten.h>
#include <emscripten/html5.h>

#include <EGL/egl.h>

#include <GLES3/gl3.h>

#include <iostream>
#include <format>
#include <cassert>


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
    NApachee::NSimple3D::NWebGL2::Initialize();

    glViewport(0, 0, 800, 600);

// #ifndef NDEBUG
// #endif

    // emscripten_set_main_loop();
    emscripten_set_main_loop(&main_loop, 0, true);


    // assert(eglInitialize(display, nullptr, nullptr));

    // EGLConfig* configs{};

}
