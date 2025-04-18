#include <simple3d/internal/frontend/graphics/renderer.h>
#include <simple3d/internal/frontend/library/webgl2/webgl.h>

#include <simple3d/core/assert.h>
#include <simple3d/core/static_initializer.h>


#include <emscripten.h>
#include <emscripten/html5.h>

#include <EGL/egl.h>

#include <GLES3/gl3.h>

#include <exception>
#include <iostream>
#include <format>
#include <cassert>

#ifndef __EMSCRIPTEN_PTHREADS__
#error Support for pthread must be enabled
#endif

using namespace NApachee::NSimple3D;

void TerminateHandler()
{
    NWebGL2::Terminate();
    std::abort();
}

S3D_STATIC_INITIALIZER([] () {
    std::set_terminate(TerminateHandler);
} ());

int main()
{
    // XXX(apachee): Maybe use NApachee::NSimple3D::NFrontnend? 

    NWebGL2::InitializeOrCrash();

    NGraphics::SetMainLoop();
}
