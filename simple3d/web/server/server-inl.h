#ifndef S3D_SERVER_INL_H_
#error This file should not be included directly, use .h version
#include "server.h"
#endif

#include <simple3d/core/assert.h>
#include <simple3d/core/types.h>

#include <uWebSockets/HttpParser.h>
#include <uWebSockets/WebSocketProtocol.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <format>

// TODO(apachee): Fix this mess: weird code for static files, random headers without proper understanding,
// which can potentially lead to some problems.

namespace NSimple3D::NWebServer {

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebServer
