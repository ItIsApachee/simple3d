#pragma once

namespace NSimple3D {

////////////////////////////////////////////////////////////////////////////////

#define S3D_STATIC_INITIALIZER(...) \
    [[maybe_unused]] static inline const void* StaticInitializer ## __COUNTER__ = [] { \
        __VA_ARGS__; \
        return nullptr; \
    } ()

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D
