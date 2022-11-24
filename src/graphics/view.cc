#include <simple3d/graphics/view.h>

#include <simple3d/graphics/scene.h>

namespace Simple3D {



View::View() {
    // FIXME: create framebuffer
}

View::~View() {
    // FIXME: destroy framebuffer
}

void View::Draw(const Scene &scene) {
    // FIXME: bind framebuffer
    for (auto &renderer: scene.renderers_) {
        renderer->Draw();
    }
}



}