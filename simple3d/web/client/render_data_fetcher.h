#pragma once

#include "public.h"

#include "renderer_agent.h"

#include <memory>

namespace NSimple3D::NWebClient {

////////////////////////////////////////////////////////////////////////////////

struct IRenderDataFetcher
    : public std::enable_shared_from_this<IRenderDataFetcher>
{
    virtual std::shared_ptr<TRenderData> Fetch() = 0;

    virtual ~IRenderDataFetcher() = default;
};

////////////////////////////////////////////////////////////////////////////////

IRenderDataFetcherPtr CreateRenderDataFetcher();

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebClient
