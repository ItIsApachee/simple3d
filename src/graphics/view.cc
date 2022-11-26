#include <simple3d/graphics/view.h>

#include <simple3d/graphics/scene.h>

namespace Simple3D {



View::View() {
  // FIXME(apachee): create framebuffer
}

View::~View() {
  // FIXME(apachee): destroy framebuffer
}

void View::Draw(const Scene &scene) {
  // FIXME(apachee): bind framebuffer
  for (auto &renderer : scene.renderers_) {
    renderer->Draw();
  }
}



}  // namespace Simple3D
