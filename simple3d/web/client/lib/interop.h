#pragma once

#include <string>

namespace NSimple3D::NWebClient {

////////////////////////////////////////////////////////////////////////////////

void Simple3DInitWebSocket(std::string url);

///////////////////////////////////////////////////////////////////////////////

//! If Simple3DInitWebSocket was already called,
//! returns websocket url.
//! Otherwise, blocks execution until it called,
//! and then proceeds to return websocket url.
std::string GetInitWebSocketOptions();

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebClient
