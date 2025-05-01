#ifndef S3D_CORE_ERROR_H_
#error "This file should not be included directly, use .h version"
#include "error.h"
#endif

#include <ranges>

namespace NSimple3D {

////////////////////////////////////////////////////////////////////////////////

template <typename... Args>
TError::TErrorOr(std::format_string<Args...> fmt, Args&&... args)
    : TErrorOr(std::format(std::move(fmt), std::forward<Args>(args)...), DisableFormat)
{ }

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D

////////////////////////////////////////////////////////////////////////////////

template <class TParseCtx>
constexpr TParseCtx::iterator std::formatter<NSimple3D::TError, char>::parse(TParseCtx& ctx)
{
    auto it = ctx.begin();
    if (it == ctx.end() || *it == '}') {
        return it;
    }
    throw std::format_error("Format spec is not supported for NSimple3D::TError");
}

template <class TFmtCtx>
TFmtCtx::iterator std::formatter<NSimple3D::TError, char>::format(const NSimple3D::TError& error, TFmtCtx& ctx) const
{
    constexpr std::string_view OKMessage = "OK";

    std::string_view message;
    if (error.IsOk()) {
        message = OKMessage;
    } else {
        message = error.GetMessage();
    }

    return std::ranges::copy(message, ctx.out()).out;
}

////////////////////////////////////////////////////////////////////////////////
