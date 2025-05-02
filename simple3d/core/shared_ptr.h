#pragma once

#include <memory>

namespace NSimple3D {

////////////////////////////////////////////////////////////////////////////////

#define S3D_DECLARE_REFCOUNTED_CLASS(type) \
    class type; \
    S3D_DECLARE_REFCOUNTED_TYPE(type);

#define S3D_DECLARE_REFCOUNTED_STRUCT(type) \
    struct type; \
    S3D_DECLARE_REFCOUNTED_TYPE(type);

#define S3D_DECLARE_REFCOUNTED_TYPE(type) \
    using type ## Ptr = std::shared_ptr<type>; \
    static_assert(true)

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::shared_ptr<T> MakeStrong(T* this_);

template <typename T>
std::weak_ptr<T> MakeWeak(T* this_);

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D

#define S3D_CORE_SHARED_PTR_INL_H_
#include "shared_ptr-inl.h"
#undef S3D_CORE_ERROR_PTR_INL_H_
