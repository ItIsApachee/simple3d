#include <simple3d/core/assert.h>
#include <simple3d/core/compiler.h>
#include <simple3d/core/public.h>
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

template <typename T>
TErrorOr<T>::TErrorOr()
{
    Value_.emplace();
}

template <typename T>
TErrorOr<T>::TErrorOr(TError error)
    : TError(std::move(error))
{ }

template <typename T>
TErrorOr<T>::TErrorOr(const T& value)
    : Value_(value)
{ }

template <typename T>
TErrorOr<T>::TErrorOr(T&& value)
    : Value_(std::move(value))
{ }

template <typename T>
template <typename U>
TErrorOr<T>::TErrorOr(const TErrorOr<U>& other)
    : TError(other)
{
    if (other.IsOk()) {
        Value_.emplace(other.Value());
    }
}

template <typename T>
template <typename U>
TErrorOr<T>::TErrorOr(TErrorOr<U>&& other)
    : TError(other)
{
    if (other.IsOk()) {
        Value_.emplace(std::move(other.Value()));
    }
}

template <typename T>
TErrorOr<T>::~TErrorOr() = default;

template <typename T>
TErrorOr<T>& TErrorOr<T>::operator=(const TErrorOr<T>& other)
    requires std::is_copy_assignable_v<T>
{
    static_cast<TError&>(*this) = other;
    Value_ = other.Value();
    return *this;
}

template <typename T>
TErrorOr<T>& TErrorOr<T>::operator=(TErrorOr<T>&& other)
    requires std::is_move_assignable_v<T>
{
    static_cast<TError&>(*this) = other;
    Value_ = std::move(other.Value());
    return *this;
}

template <typename T>
const T& TErrorOr<T>::Value() const & S3D_LIFETIME_BOUND
{
    S3D_ASSERT(IsOk());
    return *Value_;
}

template <typename T>
T& TErrorOr<T>::Value() & S3D_LIFETIME_BOUND
{
    S3D_ASSERT(IsOk());
    return *Value_;
}

template <typename T>
T&& TErrorOr<T>::Value() && S3D_LIFETIME_BOUND
{
    S3D_ASSERT(IsOk());
    // NB(apachee): Uses move ctor of T type, not std::optional<T>.
    return std::move(*Value_);
}

template <typename T>
const T& TErrorOr<T>::ValueOrThrow() const & S3D_LIFETIME_BOUND
{
    ThrowOnError();
    S3D_ASSERT(IsOk());
    return Value();
}

template <typename T>
T& TErrorOr<T>::ValueOrThrow() & S3D_LIFETIME_BOUND
{
    ThrowOnError();
    S3D_ASSERT(IsOk());
    return Value();
}

template <typename T>
T&& TErrorOr<T>::ValueOrThrow() && S3D_LIFETIME_BOUND
{
    ThrowOnError();
    S3D_ASSERT(IsOk());
    return std::move(Value());
}

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

template<typename T>
    requires NSimple3D::NConcepts::CFormattable<T>
template <class TParseCtx>
constexpr TParseCtx::iterator std::formatter<NSimple3D::TErrorOr<T>, char>::parse(TParseCtx& ctx)
{
    return ValueFormatter.template parse<TParseCtx>(ctx);
}

template<typename T>
    requires NSimple3D::NConcepts::CFormattable<T>
template <class TFmtCtx>
TFmtCtx::iterator std::formatter<NSimple3D::TErrorOr<T>, char>::format(const NSimple3D::TErrorOr<T>& errorOrValue, TFmtCtx& ctx) const
{
    if (errorOrValue.IsOk()) {
        return ValueFormatter.template format<TFmtCtx>(errorOrValue.Value(), ctx);
    } else {
        return std::formatter<NSimple3D::TError, char>().format(errorOrValue, ctx);
    }
}

////////////////////////////////////////////////////////////////////////////////
