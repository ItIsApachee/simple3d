#include "server.h"

#include <simple3d/core/graphics/primitive.h>

#include <simple3d/core/compiler.h>

#include <uWebSockets/App.h>

#include <chrono>
#include <thread>

namespace NSimple3D::NWebServer {

using namespace NGraphics;

////////////////////////////////////////////////////////////////////////////////

int Main()
{
    auto app = TApp();
    app.Run();

    {
        auto renderData = std::make_shared<TRenderData>(TRenderData{
            TTriangle{
                .Model = glm::mat4(1.0f),
                .Vertices = {
                    TVertex{
                        .Pos = glm::vec3(-0.5f, -0.5f, 0.0f),
                    },
                    TVertex{
                        .Pos = glm::vec3(-0.5f, 0.5f, 0.0f),
                    },
                    TVertex{
                        .Pos = glm::vec3(0.5f, -0.5f, 0.0f),
                    },
                },
            },
            TTriangle{
                .Model = glm::mat4(1.0f),
                .Vertices = {
                    TVertex{
                        .Pos = glm::vec3(0.5f, 0.5f, 0.0f),
                    },
                    TVertex{
                        .Pos = glm::vec3(-0.5f, 0.5f, 0.0f),
                    },
                    TVertex{
                        .Pos = glm::vec3(0.5f, -0.5f, 0.0f),
                    },
                },
            },
        });
        app.UpdateRenderData(renderData);
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebServer

int main() {
    return ::NSimple3D::NWebServer::Main();
}
