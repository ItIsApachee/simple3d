#ifndef S3D_CORE_SHARED_PTR_INL_H_
#error This file should not be included directly, use .h version
#include "shared_ptr.h"
#endif

#include "assert.h"

#include <memory>

namespace NSimple3D {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::shared_ptr<T> MakeStrong(T* this_)
{
    if (!this_) {
        return {};
    }

    auto result = std::dynamic_pointer_cast<T>(this_->shared_from_this());
    S3D_VERIFY(result);
    return result;
}

template <typename T>
std::weak_ptr<T> MakeWeak(T* this_)
{
    // XXX(apachee): Best I can do without delving in too deep.
    return MakeStrong(this_);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D
