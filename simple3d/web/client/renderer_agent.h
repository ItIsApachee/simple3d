#pragma once

#include "public.h"

#include <simple3d/web/client/lib/graphics/model_shader.h>

#include <emscripten.h>

#include <memory>
#include <vector>

namespace NSimple3D::NWebClient {

////////////////////////////////////////////////////////////////////////////////

struct IRendererAgent
    : public std::enable_shared_from_this<IRendererAgent>
{
    virtual void Start() = 0;

    virtual ~IRendererAgent() = default;
};

////////////////////////////////////////////////////////////////////////////////

IRendererAgentPtr CreateRendererAgent(TRendererAgentConfigPtr config);

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebClient
