#include <simple3d/graphics/view.h>

#include <glad/gles2.h>
#include <glm/mat4x4.hpp>

#include <simple3d/graphics/scene.h>
#include <simple3d/graphics/camera.h>

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
  glClearColor(0., 0., 0., 1.0f);

  auto& cam = scene.active_camera_;
  auto proj = cam->GetProj();
  auto view = cam->GetView();
  for (auto &renderer : scene.renderers_) {
    renderer->Draw(view, proj);
  }

  glFlush();
}



}  // namespace Simple3D
