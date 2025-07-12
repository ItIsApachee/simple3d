#pragma once

#include "public.h"

#include <simple3d/core/graphics/primitive.h>

#include <memory>

namespace NSimple3D::NWebClient {

////////////////////////////////////////////////////////////////////////////////

struct IRenderDataFetcher
    : public std::enable_shared_from_this<IRenderDataFetcher>
{
    virtual std::shared_ptr<NGraphics::TRenderData> Fetch() = 0;

    virtual ~IRenderDataFetcher() = default;
};

////////////////////////////////////////////////////////////////////////////////

IRenderDataFetcherPtr CreateRenderDataFetcher();

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebClient
