#ifndef SIMPLE3D_GRAPHICS_PRIMITIVE_PRIMITIVE_H
#define SIMPLE3D_GRAPHICS_PRIMITIVE_PRIMITIVE_H

#include <iostream>
#include <vector>
#include <type_traits>
#include <cstdint>

#include <simple3d/types.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/scene.h>

namespace Simple3D {



// definition
template <typename P>
class Primitive {
public:
    explicit Primitive(P* p_);
    Primitive() = delete;
    Primitive(const Primitive&) = delete;
    Primitive(Primitive&&) = default;
    Primitive& operator=(const Primitive&) = delete;
    Primitive& operator=(Primitive&&) = default;
    // FIXME: add destroying primitive on the scene on desctruction
    // one way to do that:
    ~Primitive() = default;

    P& operator*();
    const P& operator*() const;
    P* operator->();
    const P* operator->() const;

    // should all primitives of the same type be associated with some unique id?
    // std::uint64_t Id();
private:
    P* primitive_{nullptr};
    // std::uint64_t id_{0};
};

// TODO: add is_primitive trait
// FIXME: remove test primitive
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
template <typename P>
Primitive<P>::Primitive(P* p_): primitive_{p_} {}

template <typename P>
P& Primitive<P>::operator*() {
    return *primitive_;
}

template <typename P>
const P& Primitive<P>::operator*() const {
    return *primitive_;
}

template <typename P>
P* Primitive<P>::operator->() {
    return primitive_;
}

template <typename P>
const P* Primitive<P>::operator->() const {
    return primitive_;
}

template <>
TestPrimitive* TestPrimitiveRenderer::Create<TestPrimitive>() {
    std::cout << "basic create" << std::endl;
    primitives_.push_back(new TestPrimitive{-1});
    return primitives_.back();
}



}

#endif