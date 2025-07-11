#include "render_data_fetcher.h"

#include "renderer_agent.h"

#include <chrono>
#include <simple3d/web/client/lib/graphics/model_shader.h>

#include <simple3d/web/client/lib/io.h>

#include <simple3d/core/shared_ptr.h>

#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

namespace NSimple3D::NWebClient {

using namespace NGraphics;

////////////////////////////////////////////////////////////////////////////////

S3D_DECLARE_REFCOUNTED_CLASS(TRenderDataFetcher);

class TRenderDataFetcher
    : public IRenderDataFetcher
{
private:
    struct TPrivateTag
    { };

public:
    static TRenderDataFetcherPtr Create()
    {
        auto result = std::make_shared<TRenderDataFetcher>(TPrivateTag{});
        result->Initialize();
        return result;
    }

    TRenderDataFetcher(TPrivateTag)
    { }

    std::shared_ptr<TRenderData> Fetch() override
    {
        std::lock_guard<std::mutex> guard(RenderDataMutex_);
        return LastRenderData_;
    }

private:
    void Initialize()
    {
        LastRenderData_ = std::make_shared<TRenderData>(TRenderData{
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
            // TTriangle{
            //     .Model = glm::mat4(1.0f),
            //     .Vertices = {
            //         TVertex{
            //             .Pos = glm::vec3(0.5f, 0.5f, 0.0f),
            //         },
            //         TVertex{
            //             .Pos = glm::vec3(-0.5f, 0.5f, 0.0f),
            //         },
            //         TVertex{
            //             .Pos = glm::vec3(0.5f, -0.5f, 0.0f),
            //         },
            //     },
            // },
        });

        std::lock_guard<std::mutex> guard(FetcherThreadMutex_);
        FetcherThread_ = std::thread([this, strongThis = MakeStrong(this)] {
            {
                // Since mutex is locked in initialize, this code waits for initialize completion.
                std::lock_guard<std::mutex> innerGuard(FetcherThreadMutex_);
            }
            try {
                LoopInitialize();
                while (true) {
                    LoopIteration();
                    // emscripten_sleep(10);
                    std::this_thread::yield();
                }
            } catch (const std::exception& ex) {
                std::cout << "render data fetcher ex: " << ex.what() << std::endl;
            } catch (...) {
                std::cout << "render data fetcher unknown ex" << std::endl;
            }
        });
    }

    void LoopInitialize()
    {
        WebSocket_ = TWebSocket::Create("ws://localhost:8080/ws", "simple3d", FetcherThread_);
    }

    void LoopIteration()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        static const std::string hello = "abcde";
        auto data = std::span{reinterpret_cast<const std::byte*>(hello.data()), hello.size()};
        WebSocket_->SendBinary(data);
        auto dataCopy = WebSocket_->ReadExactlyN(data.size_bytes());

        auto dataStr = std::string((char*)data.data(), (char*)(data.data() + data.size()));
        auto dataCopyStr = std::string((char*)dataCopy.data(), (char*)(dataCopy.data() + dataCopy.size()));

        std::string dataCopyVStr;
        {
            for (auto i : dataCopy) {
                dataCopyVStr += std::format("{}, ", int(i));
            }
        }

        std::cout << std::format("ws dbg {} {}", dataStr, dataCopyVStr) << std::endl;
        while (dataCopyStr != dataStr) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    std::mutex FetcherThreadMutex_;
    std::thread FetcherThread_;

    mutable std::mutex RenderDataMutex_;
    std::shared_ptr<TRenderData> LastRenderData_;
    TWebSocketPtr WebSocket_;
};

////////////////////////////////////////////////////////////////////////////////

IRenderDataFetcherPtr CreateRenderDataFetcher()
{
    return TRenderDataFetcher::Create();
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebClient
