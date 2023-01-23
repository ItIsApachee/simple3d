/** \~Russian
 * \file
 * \brief Объект Simple3D::ShaderStorage.
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_SHADER_STORAGE_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_SHADER_STORAGE_H_

#include <simple3d/graphics/shader.h>
#include <simple3d/misc/error.h>

#include <memory>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>

namespace Simple3D {

// definition
/** \~Russian
 * \class ShaderStorage
 * \brief Хранилище шейдеров (синглтон).
 * 
 * Для оптимизации производительности, а именно уменьшение количества
 * смены shader program OpenGL, данная библиотека создает только 1
 * экземпляр каждого используемого типа шейдеров. Данный объект используется
 * для создания и хранения данных экземпляров.
*/
class ShaderStorage {
 public:
  /** \~Russian
   * \brief Получение экземпляра синглтона.
   * \return Экземпляр синглтона.
  */
  static ShaderStorage& GetInstance();

  ShaderStorage(const ShaderStorage&) = delete;
  ShaderStorage(ShaderStorage&&) = delete;
  ShaderStorage& operator=(const ShaderStorage&) = delete;
  ShaderStorage& operator=(ShaderStorage&&) = delete;
  ~ShaderStorage() = default;

  /** \~Russian
   * \brief Инициализация шейдера типа S с аргументами args.
   * \tparam S Тип шейдера.
   * \tparam Args Типы параметров для инициализации шейдера.
   * \param[in] args Параметры для инициализации шейдера.
   * \return Error::Ok() в случае удачной инициализации, иначе ошибка.
   * Если шейдер типа S уже был инициализирован, то будет возвращена ошибка
   * типа Simple3D::ErrorType::kAlreadyInitialized.
  */
  template <typename S, typename... Args>
  Error InitShader(Args&&... args);

  // if not initialized, use default constructor to initialize
  /** \~Russian
   * \brief Метод для получения шейдера типа S.
   * \tparam S Тип шейдера.
   * \return weak указатель на хранимый шейдер типа S.
   * 
   * В случае, если шейдер не был инициализирован, он инициализируется, как
   * S{};
  */
  template <typename S,
            std::enable_if_t<std::is_default_constructible_v<S>, bool> = true>
  std::weak_ptr<IShader> GetShader();

  // if not initialized, return empty weak_ptr
  /** \~Russian
   * \brief Метод для получения шейдера типа S.
   * \tparam S Тип шейдера.
   * \return weak указатель на хранимый шейдер типа S.
   * 
   * В случае, если шейдер не был инициализирован, возвращается пустой
   * указатель.
  */
  template <typename S,
            std::enable_if_t<!std::is_default_constructible_v<S>, bool> = true>
  std::weak_ptr<IShader> GetShader();

  // FIXME: call Clear() in App::Destroy()
  /** \~Russian
   * \brief Метод для полной очистки хранилища шейдеров.
  */
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
  shaders_[typeid(S)] = std::dynamic_pointer_cast(std::move(shader_ptr));
}

template <typename S,
          std::enable_if_t<std::is_default_constructible_v<S>, bool>>
std::weak_ptr<IShader> ShaderStorage::GetShader() {
  if (auto it = shaders_.find(typeid(S)); it != shaders_.end()) {
    return it->second;
  }

  auto shader_ptr = std::make_shared<S>();

  // store the pointer, and return the weak version of it
  // shared_ptr will be destroyed anyways, so move it
  return shaders_[typeid(S)] =
             std::dynamic_pointer_cast<IShader>(std::move(shader_ptr));
}

template <typename S,
          std::enable_if_t<!std::is_default_constructible_v<S>, bool>>
std::weak_ptr<IShader> ShaderStorage::GetShader() {
  if (auto it = shaders_.find(typeid(S)); it != shaders_.end()) {
    return it->second;
  }
  return {};
}

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_SHADER_STORAGE_H_
