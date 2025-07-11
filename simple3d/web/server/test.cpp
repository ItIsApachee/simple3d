#include <uWebSockets/App.h>

#include <fstream>
#include <sstream>

void AddHeadersForThreads(auto* res)
{
    res->writeHeader("Cross-Origin-Opener-Policy", "same-origin");
    res->writeHeader("Cross-Origin-Resource-Policy", "cross-origin");
    res->writeHeader("Cross-Origin-Embedder-Policy", "require-corp");
}

// TODO(apachee): Fix this mess: weird code for static files, random headers without proper understanding,
// which can potentially lead to some problems.

int main() {
    auto indexHtmlHandler = [](auto* res, uWS::HttpRequest* /*req*/) {
        std::ifstream index_html_stream("/Users/apachee/projects/simple3d/build/simple3d/web/client/out/index.html");
        std::stringstream index_html_str;
        index_html_str << index_html_stream.rdbuf();
        res->writeHeader("Content-Type", "text/html; charset=utf-8");
        AddHeadersForThreads(res);
        res->writeStatus("200 OK");
        res->write(index_html_str.str());
        res->end("");
    };

    struct TPerSocketData
    { };

    auto webApp = uWS::App()
        .get("/", indexHtmlHandler)
        .get("/index.html", indexHtmlHandler)
        .get("/index.js", [](auto* res, uWS::HttpRequest* /*req*/) {
            std::ifstream index_js_stream("/Users/apachee/projects/simple3d/build/simple3d/web/client/out/index.js");
            std::stringstream index_js_str;
            index_js_str << index_js_stream.rdbuf();

            res->writeStatus("200 OK");
            res->writeHeader("Content-Type", "text/javascript; charset=utf-8");
            AddHeadersForThreads(res);
            res->write(index_js_str.str());
            res->end("");
        })
        .get("/index.wasm", [](auto* res, uWS::HttpRequest* /*req*/) {
            std::ifstream index_wasm_stream("/Users/apachee/projects/simple3d/build/simple3d/web/client/out/index.wasm");
            std::stringstream index_wasm_str;
            index_wasm_str << index_wasm_stream.rdbuf();

            res->writeStatus("200 OK");
            res->writeHeader("Content-Type", "application/wasm");
            AddHeadersForThreads(res);
            res->write(index_wasm_str.str());
            res->end("");
        })
        .ws<TPerSocketData>("/ws", {
            .open = [] (auto* /*ws*/) {
                std::cout << "websocket open" << std::endl;
            },
            .message = [] (auto* ws, std::string_view msg, uWS::OpCode /*opCode*/) {
                std::cout << "message: " << msg << std::endl;
                ws->send("abcde");
            }
        })
        .listen(8080, [] (auto* /*sock*/) {
            std::cout << "listening on http://localhost:8080/" << std::endl;
        })
        .run();
}
