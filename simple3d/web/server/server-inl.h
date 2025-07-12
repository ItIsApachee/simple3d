#ifndef S3D_SERVER_INL_H_
#error This file should not be included directly, use .h version
#include "server.h"
#endif

#include <simple3d/core/assert.h>

#include <uWebSockets/HttpParser.h>
#include <uWebSockets/WebSocketProtocol.h>

#include <iostream>
#include <fstream>
#include <sstream>

// TODO(apachee): Fix this mess: weird code for static files, random headers without proper understanding,
// which can potentially lead to some problems.

namespace NSimple3D::NWebServer {

////////////////////////////////////////////////////////////////////////////////

template <typename TWSApp>
TApp<TWSApp>::TApp()
    : TWSApp()
{ }

////////////////////////////////////////////////////////////////////////////////

template <typename TWSApp>
TApp<TWSApp>::TApp(TWSApp wsApp)
    : TWSApp(std::move(wsApp))
{ }

////////////////////////////////////////////////////////////////////////////////

void AddHeadersForThreads(auto* res)
{
    res->writeHeader("Cross-Origin-Opener-Policy", "same-origin");
    res->writeHeader("Cross-Origin-Resource-Policy", "cross-origin");
    res->writeHeader("Cross-Origin-Embedder-Policy", "require-corp");
}

////////////////////////////////////////////////////////////////////////////////

template <typename TWSApp>
TApp<TWSApp> TApp<TWSApp>::RegisterEndpoints()
{
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

    return (*this)
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
        });
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

template <typename TWSApp>
TApp<TWSApp> TApp<TWSApp>::RegisterWebSocket()
{
    struct TPerSocketData
    { };

    auto secret = GenerateRandomString(8);

    std::cout << "ws secret: " << secret << std::endl;

    auto endpointUri = std::format("/ws/{}", secret);
    auto endpoint = std::format("ws://localhost:8080{}", endpointUri);

    std::cout << "ws endpoint: " << endpoint << std::endl;

    return (*this)
        .template ws<TPerSocketData>(endpointUri, {
            .open = [] (auto* /*ws*/) {
                std::cout << "websocket open" << std::endl;
            },
            .message = [] (auto* ws, std::string_view msg, uWS::OpCode /*opCode*/) {
                std::cout << "message: " << msg << std::endl;
                ws->send("abcde");
            }
        });
}

////////////////////////////////////////////////////////////////////////////////

template <typename TWSApp>
TApp<TWSApp> TApp<TWSApp>::ListenAndRun(int port)
{
    return (*this)
        .listen(port, [port] (auto* /*sock*/) {
            std::cout << std::format("listening on http://localhost:{}/", port) << std::endl;
        })
        .run();
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebServer
