#include "public.h"

#include "bootstrap.h"
#include "config.h"

#include <memory>

// #include <simple3d/web/client/lib/webgl2/webgl.h>

// #include <simple3d/core/assert.h>

// #include <emscripten.h>
// #include <emscripten/html5.h>

// #include <GLES3/gl3.h>

// #include <exception>
// #include <iostream>
// #include <format>
// #include <cassert>

#ifndef __EMSCRIPTEN_PTHREADS__
#error Support for pthread must be enabled
#endif

namespace NSimple3D::NWebClient {

////////////////////////////////////////////////////////////////////////////////

class TProgram
{
public:
    TProgram() = default;

    void Run() {
        Bootstrap_ = CreateBootstrap(GetConfig());

        Bootstrap_->Start();
    }

    TBootstrapConfigPtr GetConfig()
    {
        return std::make_shared<TBootstrapConfig>(TBootstrapConfig{
            .RendererAgent = std::make_shared<TRendererAgentConfig>(TRendererAgentConfig{

            }),
        });
    }

private:
    IBootstrapPtr Bootstrap_;
};


////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebClient

int main()
{
    auto program = new NSimple3D::NWebClient::TProgram();
    program->Run();

    // XXX(apachee): Do I really need this?
    [[maybe_unused]] volatile auto vProgram = program;
}
