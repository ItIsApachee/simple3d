#include <simple3d/graphics/view.h>

#include <glad/gles2.h>

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
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0., 0., 0., 1.);

  for (auto &renderer : scene.renderers_) {
    renderer->Draw();
  }

  glFlush();
}



}  // namespace Simple3D
