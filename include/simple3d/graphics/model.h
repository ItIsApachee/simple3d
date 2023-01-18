#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_

#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>
#include <cstdint>

#include <simple3d/types.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/scene.h>

namespace Simple3D {



// definition
template <typename M>
class Model {
 public:
  Model(M* p_, std::weak_ptr<IRenderer> renderer);
  Model() = default;
  Model(const Model&) = delete;
  Model(Model&&);
  Model& operator=(const Model&) = delete;
  Model& operator=(Model&&);

  // FIXME: add destroying primitive on the scene on desctruction
  // one way to do that:
  // FIXME: redefine copy ctor & assignment to set the pointer to nullptr
  ~Model();

  M& operator*();
  const M& operator*() const;
  M* operator->();
  const M* operator->() const;

  // should all primitives of the same type be associated with some unique id?
  // std::uint64_t Id();

 private:
  M* model_{nullptr};
  std::weak_ptr<IRenderer> renderer_{};
  // std::uint64_t id_{0};
};

// TODO(apachee): add is_primitive trait
// FIXME(apachee): remove test primitive
// struct TestPrimitive;
// class TestPrimitiveRenderer;

// class TestPrimitiveRenderer : public IRenderer {
//  public:
//   TestPrimitiveRenderer() = default;
//   TestPrimitiveRenderer(const TestPrimitiveRenderer&) = delete;
//   TestPrimitiveRenderer(TestPrimitiveRenderer&&) = default;
//   TestPrimitiveRenderer& operator=(const TestPrimitiveRenderer&) = delete;
//   TestPrimitiveRenderer& operator=(TestPrimitiveRenderer&&) = default;
//   ~TestPrimitiveRenderer();


//   template <typename T>
//   TestPrimitive* Create();
//   template <>
//   TestPrimitive* Create<TestPrimitive>();

//   TestPrimitive* Create(int);

//   void Draw() override;
//   void Destroy(Scene* scene) override;
//  private:
//   std::vector<TestPrimitive*> primitives_{};
// };

// struct TestPrimitive {
//  public:
//   using Renderer = TestPrimitiveRenderer;
//   int id{0};
// };

// implementation
template <typename M>
Model<M>::Model(M* p_, std::weak_ptr<IRenderer> renderer)
    : model_{p_}, renderer_{std::move(renderer)} {}

template <typename M>
Model<M>::Model(Model&& other)
    : model_{other.model_}, renderer_{std::move(other.renderer_)} {
  other.model_ = nullptr;
}

template <typename M>
Model<M>& Model<M>::operator=(Model&& other) {
  if (std::addressof(other) == this)
    return *this;
  
  if (model_ != nullptr) {
    Model<M> temp{std::move(this)};
  }  // destructor is called and model_ is freed
  // *this is now empty

  std::swap(model_, other.model_);
  std::swap(renderer_, other.renderer_);
  // other is empty

  return *this;
}

template <typename M>
Model<M>::~Model() {
  if (model_ == nullptr)
    return;
  if (auto renderer = renderer_.lock()) {
    renderer->Destroy(reinterpret_cast<void*>(model_));
  }
}

template <typename M>
M& Model<M>::operator*() {
  return *model_;
}

template <typename M>
const M& Model<M>::operator*() const {
  return *model_;
}

template <typename M>
M* Model<M>::operator->() {
  return model_;
}

template <typename M>
const M* Model<M>::operator->() const {
  return model_;
}

// template <>
// TestPrimitive* TestPrimitiveRenderer::Create<TestPrimitive>() {
//   std::cout << "basic create" << std::endl;
//   primitives_.push_back(new TestPrimitive{-1});
//   return primitives_.back();
// }



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_
