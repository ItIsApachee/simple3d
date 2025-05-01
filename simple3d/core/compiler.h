#pragma once

namespace NSimple3D {

////////////////////////////////////////////////////////////////////////////////

#if defined(__clang__) && defined(__cplusplus) && defined(__has_cpp_attribute) && __has_cpp_attribute(clang::lifetimebound)
    #define S3D_LIFETIME_BOUND [[clang::lifetimebound]]
#else
    #define S3D_LIFETIME_BOUND
#endif

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D
