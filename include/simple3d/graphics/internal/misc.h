#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_MISC_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_MISC_H_

#include <cstdint>

namespace Simple3D::Internal {
  constexpr std::uint32_t kGlesInvalidBuffer = 0;

  void BindBuffer(std::uint32_t target, std::uint32_t buffer);
  void UnbindBuffer(std::uint32_t target);
}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_MISC_H_
