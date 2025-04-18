#pragma once

#include <concepts>
#include <type_traits>

namespace NApachee::NSimple3D {

////////////////////////////////////////////////////////////////////////////////

template <typename F, typename... TArgs>
requires
    std::invocable<F, TArgs...>
    && (std::is_move_constructible_v<TArgs> && ...)
    && (std::is_copy_constructible_v<TArgs> && ...)
void SetMainLoop(F mainLoopFunc, TArgs... funcArgs);

////////////////////////////////////////////////////////////////////////////////

}  // namespace NApachee::NSimple3D

#define S3D_LIB_HELPERS_INL_H_
#include "helpers-inl.h"
#undef S3D_LIB_HELPERS_INL_H_
