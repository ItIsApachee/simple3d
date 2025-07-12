#include "server.h"

#include <mutex>
#include <simple3d/core/graphics/primitive.h>
#include <string_view>

namespace NSimple3D::NWebServer {

////////////////////////////////////////////////////////////////////////////////

void AddHeadersForThreads(auto* res)
{
    res->writeHeader("Cross-Origin-Opener-Policy", "same-origin");
    res->writeHeader("Cross-Origin-Resource-Policy", "cross-origin");
    res->writeHeader("Cross-Origin-Embedder-Policy", "require-corp");
}

////////////////////////////////////////////////////////////////////////////////

std::string GenerateRandomString(int bytes)
{
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
    S3D_VERIFY(urandom);

    std::ostringstream ostrm;

    for (int i = 0; i < bytes; i++) {
        unsigned char byte;
        urandom.read(reinterpret_cast<char*>(&byte), 1);

        ostrm << std::format("{:02x}", byte);
    }

    return ostrm.str();
}

////////////////////////////////////////////////////////////////////////////////

void TApp::Run()
{
    RunThread_ = std::thread([this] {
        constexpr int port = 8080;

        static constexpr auto indexHtmlHandler = [](auto* res, uWS::HttpRequest* /*req*/) {
            std::ifstream index_html_stream("out/index.html");
            std::stringstream index_html_str;
            index_html_str << index_html_stream.rdbuf();
            res->writeHeader("Content-Type", "text/html; charset=utf-8");
            AddHeadersForThreads(res);
            res->writeStatus("200 OK");
            res->write(index_html_str.str());
            res->end("");
        };

        constexpr i64 MaxSendSize = 4 * 1024;

        struct TPerSocketData
        {
            std::shared_ptr<NGraphics::TRenderData> CurrentRenderData;
            i64 Offset = 0;

            std::string MessageBuffer;

            std::string_view AsBinaryData(std::size_t maxSize = std::string_view::npos) const
            {
                if (!CurrentRenderData) {
                    return std::string_view();
                }
                if (Offset == std::ssize(*CurrentRenderData) * i64(sizeof(NGraphics::TTriangle))) {
                    return std::string_view();
                }
                S3D_VERIFY(Offset <= std::ssize(*CurrentRenderData) * i64(sizeof(NGraphics::TTriangle)));

                auto data = std::string_view(reinterpret_cast<const char*>(CurrentRenderData->data() + Offset), reinterpret_cast<const char*>(CurrentRenderData->data() + CurrentRenderData->size()));
                auto dataTrimmed = data.substr(0, maxSize);
                S3D_VERIFY(dataTrimmed.size() <= data.size());
                return dataTrimmed;
            }
        };

        auto secret = GenerateRandomString(8);

        std::cout << "ws secret: " << secret << std::endl;

        auto endpointUri = std::format("/ws/{}", secret);
        auto endpoint = std::format("ws://localhost:8080{}", endpointUri);

        std::cout << "ws endpoint: " << endpoint << std::endl;

        enum struct SendStatus : int {
            BACKPRESSURE,
            SUCCESS,
            DROPPED
        };

        uWS::App()
            .get("/", indexHtmlHandler)
            .get("/index.html", indexHtmlHandler)
            .get("/index.js", [](auto* res, uWS::HttpRequest* /*req*/) {
                std::ifstream index_js_stream("out/index.js");
                std::stringstream index_js_str;
                index_js_str << index_js_stream.rdbuf();

                res->writeStatus("200 OK");
                res->writeHeader("Content-Type", "text/javascript; charset=utf-8");
                AddHeadersForThreads(res);
                res->write(index_js_str.str());
                res->end("");
            })
            .get("/index.wasm", [](auto* res, uWS::HttpRequest* /*req*/) {
                std::ifstream index_wasm_stream("out/index.wasm");
                std::stringstream index_wasm_str;
                index_wasm_str << index_wasm_stream.rdbuf();

                res->writeStatus("200 OK");
                res->writeHeader("Content-Type", "application/wasm");
                AddHeadersForThreads(res);
                res->write(index_wasm_str.str());
                res->end("");
            })
            .ws<TPerSocketData>(endpointUri, {
                .open = [] (auto* ws) {
                    std::cout << "websocket open: " << ws->getRemoteAddressAsText() << std::endl;
                },
                .message = [this] (auto* ws, std::string_view msg, uWS::OpCode /*opCode*/) {
                    // std::cout << "message: " << msg << std::endl;

                    TPerSocketData& data = *(ws->getUserData());
                    data.MessageBuffer += msg;
                    // std::cout << "current message buffer \"" << data.MessageBuffer << "\"" << std::endl;
                    using namespace std::literals::string_view_literals;
                    static auto ReadyMesasge = "ready"sv;
                    if (std::string_view(data.MessageBuffer) == ReadyMesasge) {
                        data.MessageBuffer = {};

                        data.CurrentRenderData = GetRenderData();
                        data.Offset = 0;

                        constexpr i64 HeaderSize = 16;
                        auto header = std::format("header {:8} ", data.AsBinaryData().size());
                        S3D_VERIFY(header.size() == HeaderSize);

                        // std::cout << std::format("sending header \"{}\"", header) << std::endl;

                        if (ws->send(header) != static_cast<int>(SendStatus::SUCCESS)) {
                            return;
                        }
                        while (data.AsBinaryData().size() > 0) {
                            auto packet = data.AsBinaryData(MaxSendSize);
                            if (ws->send(packet) != static_cast<int>(SendStatus::SUCCESS)) {
                                return;
                            }
                            data.Offset += packet.size();
                        }
                    } else {
                        // std::cout << "not ready yet" << std::endl;
                        // for (auto i : std::string_view(data.MessageBuffer)) {
                        //     std::cout << int(i) << ", ";
                        // }
                        // std::cout << std::endl;
                        // for (auto i : ReadyMesasge) {
                        //     std::cout << int(i) << ", ";
                        // }
                        // std::cout << std::endl;
                    }
                },
                .dropped = [] (auto* /*ws*/, std::string_view, uWS::OpCode) {
                    std::cout << "dropped message!" << std::endl;
                },
                .drain = [] (auto* ws) {
                    TPerSocketData& data = *(ws->getUserData());
                    while (data.AsBinaryData().size() > 0) {
                        auto msg = data.AsBinaryData(MaxSendSize);
                        if (ws->send(msg) != static_cast<int>(SendStatus::SUCCESS)) {
                            return;
                        }
                        data.Offset += msg.size();
                    }
                },
            })
            .listen(port, [port] (auto* /*sock*/) {
                std::cout << std::format("listening on http://localhost:{}/", port) << std::endl;
            })
            .run();
    });
}

////////////////////////////////////////////////////////////////////////////////

void TApp::UpdateRenderData(std::shared_ptr<NGraphics::TRenderData> renderData)
{
    std::lock_guard guard(RenderDataMutex_);
    RenderData_ = std::move(renderData);
}

////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<NGraphics::TRenderData> TApp::GetRenderData()
{
    std::lock_guard guard(RenderDataMutex_);
    return RenderData_;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebServer
