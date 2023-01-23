/** \~Russian
 * \file
 * \brief Шаблон Simple3D::ModelHandle<M>
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODEL_HANDLE_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODEL_HANDLE_H_

#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/scene.h>
#include <simple3d/types.h>

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace Simple3D {

// definition
/** \~Russian
 * \class ModelHandle
 * \brief Обертка над некоторой моделью M.
 * \tparam M Модель.
 * 
 * Обертка над создаваемым на сцене объектом. Предоставляет доступ к
 * экземпляру M.
*/
template <typename M>
class ModelHandle {
 public:
  ModelHandle(M* p_, std::weak_ptr<IRenderer> renderer);
  ModelHandle() = default;
  ModelHandle(const ModelHandle&) = delete;
  ModelHandle(ModelHandle&&);
  ModelHandle& operator=(const ModelHandle&) = delete;
  ModelHandle& operator=(ModelHandle&&);

  ~ModelHandle();

  /** \~Russian
   * \brief Перегрузка оператора разыменовывания.
   * \return Ссылка на хранимую модель.
  */
  M& operator*();

  /** \~Russian
   * \brief Перегрузка оператора разыменовывания.
   * \return Ссылка на хранимую модель.
  */
  const M& operator*() const;

  /** \~Russian
   * \brief Перегрузка оператора доступа к полям и методам по указателю.
   * \return Ссылка на хранимую модель.
  */
  M* operator->();

  /** \~Russian
   * \brief Перегрузка оператора доступа к полям и методам по указателю.
   * \return Ссылка на хранимую модель.
  */
  const M* operator->() const;

  // should all primitives of the same type be associated with some unique id?
  // std::uint64_t Id();

 private:
  M* model_{nullptr};
  std::weak_ptr<IRenderer> renderer_{};
  // std::uint64_t id_{0};
};

// implementation
template <typename M>
ModelHandle<M>::ModelHandle(M* p_, std::weak_ptr<IRenderer> renderer)
    : model_{p_}, renderer_{std::move(renderer)} {}

template <typename M>
ModelHandle<M>::ModelHandle(ModelHandle&& other)
    : model_{other.model_}, renderer_{std::move(other.renderer_)} {
  other.model_ = nullptr;
}

template <typename M>
ModelHandle<M>& ModelHandle<M>::operator=(ModelHandle&& other) {
  if (std::addressof(other) == this) return *this;

  if (model_ != nullptr) {
    ModelHandle<M> temp = std::move(*this);
  }  // destructor is called and model_ is freed
  // *this is now empty

  model_ = other.model_;
  other.model_ = nullptr;
  renderer_ = std::move(other.renderer_);
  // other is empty

  return *this;
}

template <typename M>
ModelHandle<M>::~ModelHandle() {
  if (model_ == nullptr) return;
  if (auto renderer = renderer_.lock()) {
    renderer->Destroy(reinterpret_cast<void*>(model_));
  }
}

template <typename M>
M& ModelHandle<M>::operator*() {
  if (auto renderer = renderer_.lock()) {
    renderer->NotifyUpdated(reinterpret_cast<void*>(model_));
  }
  return *model_;
}

template <typename M>
const M& ModelHandle<M>::operator*() const {
  return *model_;
}

template <typename M>
M* ModelHandle<M>::operator->() {
  if (auto renderer = renderer_.lock()) {
    renderer->NotifyUpdated(reinterpret_cast<void*>(model_));
  }
  return model_;
}

template <typename M>
const M* ModelHandle<M>::operator->() const {
  return model_;
}

// template <>
// TestPrimitive* TestPrimitiveRenderer::Create<TestPrimitive>() {
//   std::cout << "basic create" << std::endl;
//   primitives_.push_back(new TestPrimitive{-1});
//   return primitives_.back();
// }

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODEL_HANDLE_H_
