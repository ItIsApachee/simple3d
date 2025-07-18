#pragma once

#include <source_location>
#include <string_view>

namespace NSimple3D {

////////////////////////////////////////////////////////////////////////////////

namespace NDetail {

[[noreturn]]
void AssertTrapImpl(
    std::string_view trapName,
    std::string_view expr,
    std::string_view message = {},
    std::source_location sourceLocation = std::source_location::current());

}  // namespace NDetail

#if defined(__GNUC__)
    #define S3D_BUILTIN_TRAP_IMPL() __builtin_trap()
#else
    #define S3D_BUILTIN_TRAP_IMPL() std::terminate()
#endif

#define S3D_ALERT(message) \
    do { \
        ::NSimple3D::NDetail::AssertTrapImpl("S3D_ALERT", "", message); \
    } while (false)

#define S3D_VERIFY(expr) \
    do { \
        if (!(expr)) [[unlikely]] { \
            ::NSimple3D::NDetail::AssertTrapImpl("S3D_VERIFY", #expr); \
        } \
    } while (false)

#ifdef NDEBUG
    #define S3D_ASSERT(expr) \
        do { \
            if (false) { \
                (void) (expr); \
            } \
        } while (false)
#else
    #define S3D_ASSERT(expr) \
        do { \
            if (!(expr)) [[unlikely]] { \
                ::NSimple3D::NDetail::AssertTrapImpl("S3D_ASSERT", #expr);\
            } \
        } while (false)
#endif

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D
