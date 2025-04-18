#include <simple3d/core/assert.h>

#include <iostream>
#include <format>
#include <utility>

namespace NApachee::NSimple3D::NDetail {

////////////////////////////////////////////////////////////////////////////////

void AssertTrapImpl(
    std::string_view trapName,
    std::string_view fileLoc,
    int line,
    std::string_view expr)
{
    std::cerr << std::format("at {}:{} {}({}) failed", fileLoc, line, trapName, expr) << std::endl;
    S3D_BUILTIN_TRAP_IMPL();
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NApachee::NSimple3D::NDetail
