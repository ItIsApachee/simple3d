#include "error.h"
#include <simple3d/core/public.h>

namespace NSimple3D {

////////////////////////////////////////////////////////////////////////////////

namespace {

////////////////////////////////////////////////////////////////////////////////

// XXX(apachee): Simplest approximation for the time being.
enum struct EErrorCode : int
{
    OK = 0,
    Generic = 1,
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace

////////////////////////////////////////////////////////////////////////////////

class TError::TImpl
{
public:
    TImpl() = default;

    explicit TImpl(std::string message)
        : Code_(EErrorCode::Generic)
        , Message_(std::move(message))
    { }

    TImpl(TImpl&) = default;

    TImpl(TImpl&& other)
        : Code_(other.Code_)
        , Message_(std::move(other.Message_))
    {
        other.Code_ = EErrorCode::OK;
    }

    TImpl& operator=(const TImpl&) = default;

    TImpl& operator=(TImpl&& other)
    {
        Code_ = other.Code_;
        other.Code_ = EErrorCode::OK;

        Message_ = std::move(other.Message_);

        return *this;
    }

    const std::string& GetMessage() const
    {
        return Message_;
    }

    std::string* MutableMessage()
    {
        return &Message_;
    }

    bool IsOk() const
    {
        return Code_ == EErrorCode::OK;
    }

private:
    EErrorCode Code_ = EErrorCode::OK;
    std::string Message_;
};

////////////////////////////////////////////////////////////////////////////////

TError::TErrorOr() = default;

TError::TErrorOr(const TError& other)
{
    if (!other.IsOk()) {
        Impl_ = std::make_unique<TImpl>(*other.Impl_);
    }
}

TError::TErrorOr(TError&& other)
    : Impl_(std::move(other.Impl_))
{ }

TError::TErrorOr(std::string message, TDisableFormat)
    : Impl_(std::make_unique<TImpl>(message))
{ }

TError::~TErrorOr() = default;

TError& TError::operator=(const TError& other)
{
    if (other.IsOk()) {
        Impl_.reset();
    } else {
        Impl_ = std::make_unique<TImpl>(*other.Impl_);
    }

    return *this;
}

TError& TError::operator=(TError&& other)
{
    Impl_ = std::move(other.Impl_);
    return *this;
}

const std::string& TError::GetMessage() const
{
    if (Impl_) {
        return Impl_->GetMessage();
    }

    static const std::string EmptyMessage;
    return EmptyMessage;
}

TError& TError::SetMesasge(std::string message)
{
    *Impl_->MutableMessage() = std::move(message);
    return *this;
}

bool TError::IsOk() const
{
    return !Impl_ || Impl_->IsOk();
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D
