#include <simple3d/core/assert.h>

#include <iostream>
#include <format>
#include <utility>

namespace NApachee::NSimple3D::NDetail {

////////////////////////////////////////////////////////////////////////////////

#ifdef __EMSCRIPTEN__
    #define _S3D_ASSERT_FILE_PREFIX "file://"
#else
    #define _S3D_ASSERT_FILE_PREFIX ""
#endif

void AssertTrapImpl(
    std::string_view trapName,
    std::string_view expr,
    std::string_view message,
    std::source_location sourceLocation)
{
    std::string filename = _S3D_ASSERT_FILE_PREFIX;
    filename += sourceLocation.file_name();

    if (message.empty()) [[likely]] {
        std::cerr << std::format(
            "{}({}) at {}:{}:{} in {}",
            trapName,
            expr,
            filename,
            sourceLocation.line(),
            sourceLocation.column(),
            sourceLocation.function_name());
    } else {
        std::cerr << std::format(
            "{}({}) at {}:{}:{} in {} with message: {}",
            trapName,
            expr,
            filename,
            sourceLocation.line(),
            sourceLocation.column(),
            sourceLocation.function_name(),
            message);
    }
    std::cerr << std::endl;

    S3D_BUILTIN_TRAP_IMPL();
}

#undef _S3D_ASSERT_FILE_PREFIX

////////////////////////////////////////////////////////////////////////////////

}  // namespace NApachee::NSimple3D::NDetail
