#include <simple3d/web/client/gl/renderer.h>
#include <simple3d/web/client/lib/webgl2/webgl.h>

#include <simple3d/core/assert.h>

#include <emscripten.h>
#include <emscripten/html5.h>

#include <GLES3/gl3.h>

#include <exception>
#include <iostream>
#include <format>
#include <cassert>

#ifndef __EMSCRIPTEN_PTHREADS__
#error Support for pthread must be enabled
#endif

using namespace NSimple3D;

class TProgram
{
public:
    TProgram() = default;

    void Start() {
        std::set_terminate(TerminateHandler);
        NGLib::Start();
    }

    static void TerminateHandler()
    {
        // TODO(apachee): Setup all threads and exit.
        NWebGL2::Terminate();
        std::abort();
    }
};

int main()
{
    auto program = new TProgram();
    program->Start();

    // XXX(apachee): Do I really need this?
    [[maybe_unused]] volatile auto vProgram = program;
}
