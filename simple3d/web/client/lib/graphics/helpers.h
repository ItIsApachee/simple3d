#pragma once

#include <concepts>
#include <type_traits>
#include <memory>

namespace NSimple3D::NGraphics {

////////////////////////////////////////////////////////////////////////////////

template <typename F, typename... TArgs>
requires
    std::invocable<F, TArgs...>
    && (std::is_move_constructible_v<TArgs> && ...)
    && (std::is_copy_constructible_v<TArgs> && ...)
void SetMainLoop(F mainLoopFunc, TArgs... funcArgs);

////////////////////////////////////////////////////////////////////////////////

template <typename F, typename T, typename... TArgs>
requires
    std::is_member_function_pointer_v<F>
    && (std::is_move_constructible_v<TArgs> && ...)
    && (std::is_copy_constructible_v<TArgs> && ...)
void SetMainLoop(F mainLoopFunc, std::weak_ptr<T> weakThis, TArgs... funcArgs);

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NGraphics

#define S3D_LIB_GRAPHICS_HELPERS_INL_H_
#include "helpers-inl.h"
#undef S3D_LIB_GRAPHICS_HELPERS_INL_H_
