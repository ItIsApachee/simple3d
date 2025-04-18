#include <type_traits>
#ifndef S3D_LIB_HELPERS_INL_H_
#error This file should not be included directly, use .h version
#include "helpers.h"
#endif

#include <tuple>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

namespace NApachee::NSimple3D {

////////////////////////////////////////////////////////////////////////////////

template <typename F, typename... TArgs>
requires
    std::invocable<F, TArgs...>
    && (std::is_move_constructible_v<TArgs> && ...)
    && (std::is_copy_constructible_v<TArgs> && ...)
void SetMainLoop(F mainLoopFunc, TArgs... funcArgs)
{
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

}  // namespace NApachee::NSimple3D