#include "public.h"

#include "bootstrap.h"
#include "config.h"

#include <iostream>
#include <memory>

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
        try {
            GuardedRun();
        } catch (const std::exception& ex) {
            std::cerr << std::format("Unhandled exception: {}", ex.what()) << std::endl;
        } catch (...) {
            std::cerr << std::format("Unhandled exception of unknown type") << std::endl;
        }
    }

    void GuardedRun()
    {
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
