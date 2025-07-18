#include "render_data_fetcher.h"

#include "renderer_agent.h"

#include <simple3d/web/client/lib/graphics/model_shader.h>

#include <simple3d/web/client/lib/interop.h>
#include <simple3d/web/client/lib/io.h>

#include <simple3d/core/graphics/primitive.h>

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
        LastRenderData_ = std::make_shared<TRenderData>();

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
        auto url = GetInitWebSocketOptions();
        std::cout << std::format("Recieved init websocket options url = {}", url) << std::endl;
        WebSocket_ = TWebSocket::Create(url, "simple3d", FetcherThread_);
    }

    // FIXME(apachee): Fix this mess of a protocol.
    void LoopIteration()
    {
        using namespace std::literals::string_view_literals;
        static auto ReadyMesasge = "ready"sv;

        WebSocket_->SendBinary(std::span<const std::byte>((std::byte*)ReadyMesasge.data(), (std::byte*)(ReadyMesasge.data() + ReadyMesasge.size())));
        auto header = WebSocket_->ReadExactlyN(16);
        auto headerSV = std::string_view((const char*)header.data(), (const char*)(header.data() + header.size()));

        i64 dataSize = 0;
        {
            std::istringstream istrm;
            istrm.str(std::string(headerSV));
            {
                std::string v;
                istrm >> v;
            }
            istrm >> dataSize;
        }

        auto newRenderDataRaw = WebSocket_->ReadExactlyN(dataSize);
        S3D_VERIFY(newRenderDataRaw.size() % sizeof(TTriangle) == 0);

        std::shared_ptr<TRenderData> newRenderData = std::make_shared<TRenderData>(newRenderDataRaw.size() / sizeof(TTriangle));
        std::memcpy(newRenderData->data(), newRenderDataRaw.data(), newRenderDataRaw.size());
        {
            std::lock_guard guard(RenderDataMutex_);
            LastRenderData_ = std::move(newRenderData);
        }

        std::this_thread::yield();
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
