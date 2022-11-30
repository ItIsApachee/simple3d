#include <simple3d/graphics/internal/misc.h>

#include <glad/gles2.h>

namespace Simple3D::Internal {




void BindBuffer(std::uint32_t target, std::uint32_t buffer) {
  glBindBuffer(target, buffer);
}

void UnbindBuffer(std::uint32_t target) {
  glBindBuffer(target, kGlesInvalidBuffer);
}



}  // namespace Simple3D::Internal
