#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_

#include <iostream>
#include <vector>
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
  explicit Model(M* p_);
  Model() = delete;
  Model(const Model&) = delete;
  Model(Model&&) = default;
  Model& operator=(const Model&) = delete;
  Model& operator=(Model&&) = default;
  // FIXME: add destroying primitive on the scene on desctruction
  // one way to do that:
  ~Model() = default;

  M& operator*();
  const M& operator*() const;
  M* operator->();
  const M* operator->() const;

  // should all primitives of the same type be associated with some unique id?
  // std::uint64_t Id();

 private:
  M* model_{nullptr};
  // std::uint64_t id_{0};
};

// TODO(apachee): add is_primitive trait
// FIXME(apachee): remove test primitive
struct TestPrimitive;
class TestPrimitiveRenderer;

class TestPrimitiveRenderer : public IRenderer {
 public:
  TestPrimitiveRenderer() = default;
  TestPrimitiveRenderer(const TestPrimitiveRenderer&) = delete;
  TestPrimitiveRenderer(TestPrimitiveRenderer&&) = default;
  TestPrimitiveRenderer& operator=(const TestPrimitiveRenderer&) = delete;
  TestPrimitiveRenderer& operator=(TestPrimitiveRenderer&&) = default;
  ~TestPrimitiveRenderer();


  template <typename T>
  TestPrimitive* Create();
  template <>
  TestPrimitive* Create<TestPrimitive>();

  TestPrimitive* Create(int);

  void Draw() override;
  void Destroy(Scene* scene) override;
 private:
  std::vector<TestPrimitive*> primitives_{};
};

struct TestPrimitive {
 public:
  using Renderer = TestPrimitiveRenderer;
  int id{0};
};

// implementation
template <typename M>
Model<M>::Model(M* p_): model_{p_} {}

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

template <>
TestPrimitive* TestPrimitiveRenderer::Create<TestPrimitive>() {
  std::cout << "basic create" << std::endl;
  primitives_.push_back(new TestPrimitive{-1});
  return primitives_.back();
}



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODEL_H_
