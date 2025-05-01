#pragma once

#include "public.h"

#include <format>
#include <memory>

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

    //! Construct error message by passing all formatArgs to std::format.
    template <typename... Args>
    TErrorOr(std::format_string<Args...> fmt, Args&&... args);

    ~TErrorOr();

    TErrorOr& operator=(const TErrorOr&);
    TErrorOr& operator=(TErrorOr&&);

    const std::string& GetMessage() const;
    TError& SetMesasge(std::string message);

    bool IsOk() const;

private:
    class TImpl;

    std::unique_ptr<TImpl> Impl_;
};


// TODO(apachee): Add TErrorOr<T>

// template <typename T>
// class [[nodiscard]] TErrorOr
//     : public TError
// {

// };

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

#define S3D_CORE_ERROR_H_
#include "error-inl.h"
#undef S3D_CORE_ERROR_H_
