#include <simple3d/graphics/shader_storage.h>

#include <utility>
#include <vector>

#include <simple3d/misc/error.h>
#include <simple3d/graphics/shader.h>

namespace Simple3D {



ShaderStorage& ShaderStorage::GetInstance() {
  static ShaderStorage shader_storage{};
  return shader_storage;
}

void ShaderStorage::Clear() {
  shaders_.clear();
}



}  // namespace Simple3D
