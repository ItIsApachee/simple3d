#include <simple3d/graphics/primitive.h>

#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/scene.h>

namespace Simple3D {



void TestPrimitiveRenderer::Draw() {
    for (auto& p: primitives_) {
        std::cout << "it works! (" << p->id << ")" << std::endl;
    }
}

void TestPrimitiveRenderer::Destroy(Scene* scene) {
    Internal::RendererStorage<TestPrimitiveRenderer>.erase(scene);
}

TestPrimitiveRenderer::~TestPrimitiveRenderer() {
    for (auto& i: primitives_)
        delete i;
}

TestPrimitive* TestPrimitiveRenderer::Create(int id) {
    std::cout << "create with int" << std::endl;
    primitives_.emplace_back(new TestPrimitive{id});
    return primitives_.back();
}



}