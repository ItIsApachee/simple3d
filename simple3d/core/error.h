#pragma once

#include "public.h"

#include "compiler.h"
#include "error_code.h"
#include "format.h"

#include <exception>
#include <format>
#include <memory>
#include <optional>
#include <type_traits>

namespace NSimple3D {

////////////////////////////////////////////////////////////////////////////////

template <>
class [[nodiscard]] TErrorOr<void>
{
public:
    TErrorOr();
    TErrorOr(const TErrorOr&);
    TErrorOr(TError&&);

    struct TDisableFormat
    { };
    static constexpr TDisableFormat DisableFormat = {};

    TErrorOr(std::string message, TDisableFormat);
    TErrorOr(EErrorCode code, std::string message, TDisableFormat);

    //! Construct error message by passing all formatArgs to std::format.
    template <typename... Args>
    TErrorOr(std::format_string<Args...> fmt, Args&&... args);
    template <typename... Args>
    TErrorOr(EErrorCode code, std::format_string<Args...> fmt, Args&&... args);

    ~TErrorOr();

    TErrorOr& operator=(const TErrorOr&);
    TErrorOr& operator=(TErrorOr&&);

    const std::string& GetMessage() const;
    TError& SetMessage(std::string message);

    const EErrorCode& GetCode() const;
    TError& SetCode(EErrorCode code);

    bool IsOk() const;

    void ThrowOnError() const &;
    void ThrowOnError() &&;

private:
    class TImpl;

    std::unique_ptr<TImpl> Impl_;
};

////////////////////////////////////////////////////////////////////////////////

class TErrorException
    : public std::exception
{
public:
    TErrorException(TError error);

    TErrorException(const TErrorException&) = default;
    TErrorException(TErrorException&&) = default;

    const char* what() const noexcept override;

    const TError& GetUnderlyingError() const;

private:
    TError UnderlyingError_;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class [[nodiscard]] TErrorOr
    : public TError
{
public:
    TErrorOr();

    TErrorOr(TError error);

    TErrorOr(const T& value);
    TErrorOr(T&& value);

    template <typename U>
    TErrorOr(const TErrorOr<U>& other);
    template <typename U>
    TErrorOr(TErrorOr<U>&& other);

    ~TErrorOr();

    TErrorOr& operator=(const TErrorOr<T>&)
        requires std::is_copy_assignable_v<T>;
    TErrorOr& operator=(TErrorOr<T>&&)
        requires std::is_move_assignable_v<T>;

    const T& Value() const & S3D_LIFETIME_BOUND;
    T& Value() & S3D_LIFETIME_BOUND;
    T&& Value() && S3D_LIFETIME_BOUND;

    const T& ValueOrThrow() const & S3D_LIFETIME_BOUND;
    T& ValueOrThrow() & S3D_LIFETIME_BOUND;
    T&& ValueOrThrow() && S3D_LIFETIME_BOUND;

private:
    std::optional<T> Value_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D

////////////////////////////////////////////////////////////////////////////////

template<>
struct std::formatter<NSimple3D::TError, char>
{
    template <class TParseCtx>
    constexpr TParseCtx::iterator parse(TParseCtx& ctx);

    template <class TFmtCtx>
    TFmtCtx::iterator format(const NSimple3D::TError& error, TFmtCtx& ctx) const;
};

////////////////////////////////////////////////////////////////////////////////

template<typename T>
    requires NSimple3D::NConcepts::CFormattable<T>
struct std::formatter<NSimple3D::TErrorOr<T>, char>
{
    std::formatter<T, char> ValueFormatter{};

    template <class TParseCtx>
    constexpr TParseCtx::iterator parse(TParseCtx& ctx);

    template <class TFmtCtx>
    TFmtCtx::iterator format(const NSimple3D::TErrorOr<T>& errorOrValue, TFmtCtx& ctx) const;
};

////////////////////////////////////////////////////////////////////////////////

#define S3D_CORE_ERROR_H_
#include "error-inl.h"
#undef S3D_CORE_ERROR_H_
