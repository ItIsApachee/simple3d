#ifndef INCLUDE_SIMPLE3D_GRAPHICS_SHADER_STORAGE_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_SHADER_STORAGE_H_

#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

#include <simple3d/misc/error.h>
#include <simple3d/graphics/shader.h>

namespace Simple3D {



// definition
class ShaderStorage {
 public:
  static ShaderStorage& GetInstance();

  ShaderStorage(const ShaderStorage&) = delete;
  ShaderStorage(ShaderStorage&&) = delete;
  ShaderStorage& operator=(const ShaderStorage&) = delete;
  ShaderStorage& operator=(ShaderStorage&&) = delete;
  ~ShaderStorage() = default;

  template <typename S, typename... Args>
  Error InitShader(Args&&... args);

  // if not initialized, use default constructor to initialize
  template <typename S, std::enable_if_t<std::is_default_constructible_v<S>, bool> = true>
  std::weak_ptr<IShader> GetShader();

  // if not initialized, return empty weak_ptr
  template <typename S, std::enable_if_t<!std::is_default_constructible_v<S>, bool> = true>
  std::weak_ptr<IShader> GetShader();

  void Clear();

 private:
  ShaderStorage() = default;

  std::unordered_map<std::type_index, std::shared_ptr<IShader>> shaders_{};
};



// implementation
template <typename S, typename... Args>
Error ShaderStorage::InitShader(Args&&...) {
  if (auto it = shaders_.find(typeid(S)); it != shaders_.end()) {
    return Error(ErrorType::kAlreadyInitialized, 
                 "such shader type is already initialized");
  }

  auto shader_ptr = std::make_shared<S>();

  // shared_ptr will be destroyed anyways, so move it
  shaders_[typeid(S)] = std::dynamic_pointer_cast(
      std::move(shader_ptr));
}

template <typename S, std::enable_if_t<std::is_default_constructible_v<S>, bool>>
std::weak_ptr<IShader> ShaderStorage::GetShader() {
  if (auto it = shaders_.find(typeid(S)); it != shaders_.end()) {
    return it->second;
  }

  auto shader_ptr = std::make_shared<S>();

  // store the pointer, and return the weak version of it
  // shared_ptr will be destroyed anyways, so move it
  return shaders_[typeid(S)] = std::dynamic_pointer_cast<IShader>(
      std::move(shader_ptr));
}

template <typename S, std::enable_if_t<!std::is_default_constructible_v<S>, bool>>
std::weak_ptr<IShader> ShaderStorage::GetShader() {
  if (auto it = shaders_.find(typeid(S)); it != shaders_.end()) {
    return it->second;
  }
  return {};
}



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_SHADER_STORAGE_H_