#ifndef S3D_LIB_HELPERS_INL_H_
#error This file should not be included directly, use .h version
#include "helpers.h"
#endif

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include <tuple>
#include <type_traits>
#include <functional>

namespace NSimple3D::NGraphics {

////////////////////////////////////////////////////////////////////////////////

template <typename F, typename... TArgs>
requires
    std::invocable<F, TArgs...>
    && (std::is_move_constructible_v<TArgs> && ...)
    && (std::is_copy_constructible_v<TArgs> && ...)
void SetMainLoop(F mainLoopFunc, TArgs... funcArgs)
{
    static_assert(!std::is_member_function_pointer_v<F>);

    struct TFunc
    {
        F Func;
        std::tuple<TArgs...> Args;
    };

    auto funcPtr = new TFunc{
        .Func = mainLoopFunc,
        .Args = std::tuple(std::move(funcArgs)...)
    };

    void* voidedFunc = static_cast<void*>(funcPtr);

    auto wrapperFunc = [] (void* voidedFunc) {
        TFunc* originalFunc = static_cast<TFunc*>(voidedFunc);
        std::apply(originalFunc->Func, originalFunc->Args);
    };

    emscripten_set_main_loop_arg(wrapperFunc, voidedFunc, 0, false);
}

////////////////////////////////////////////////////////////////////////////////

template <typename F, typename T, typename... TArgs>
requires
    std::is_member_function_pointer_v<F>
    && (std::is_move_constructible_v<TArgs> && ...)
    && (std::is_copy_constructible_v<TArgs> && ...)
void SetMainLoop(F mainLoopFunc, std::weak_ptr<T> weakThis, TArgs... funcArgs)
{
    static_assert(!std::invocable<F, TArgs...>);

    struct TMemberFunc
    {
        F Func;
        std::weak_ptr<T> WeakThis;
        std::tuple<TArgs...> Args;
    };

    auto funcPtr = new TMemberFunc{
        .Func = mainLoopFunc,
        .WeakThis = std::move(weakThis),
        .Args = std::tuple(std::move(funcArgs)...)
    };

    void* voidedFunc = static_cast<void*>(funcPtr);

    auto wrapperFunc = [] (void* voidedFunc) {
        TMemberFunc* originalFunc = static_cast<TMemberFunc*>(voidedFunc);

        auto strongThis = originalFunc->WeakThis.lock();
        if (strongThis) {
            std::apply(std::bind_front(originalFunc->Func, strongThis.get()), originalFunc->Args);
        }
    };

    emscripten_set_main_loop_arg(wrapperFunc, voidedFunc, 0, false);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NGraphics
