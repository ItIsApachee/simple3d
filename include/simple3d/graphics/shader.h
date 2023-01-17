#ifndef INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_

#include <unordered_map>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <simple3d/types.h>

namespace Simple3D {



class IShader {
 public:
  IShader() = default;
  virtual ~IShader() = default;

  virtual void SetView(const glm::mat4&) = 0;
  virtual void SetProj(const glm::mat4&) = 0;
  virtual void SetViewPos(const glm::vec3&) = 0;
  virtual void Use() = 0;

  // TODO(apahcee): add lighting support
};

// namespace Internal {



// template <typename S>
// class ShaderStorage {
//  public:
//   static ShaderStorage& GetInstance() {
//     static ShaderStorage shader_storage{};
//     return shader_storage;
//   }

//   ShaderStorage(const ShaderStorage&) = delete;
//   ShaderStorage(ShaderStorage&&) = delete;
//   ShaderStorage& operator=(const ShaderStorage&) = delete;
//   ShaderStorage& operator=(ShaderStorage&&) = delete;

//   std::unordered_map<Scene*, S> storage{};

//  private:
//   ShaderStorage() = default;
// };



// }



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_