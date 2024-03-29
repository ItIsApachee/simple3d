#include <glad/gles2.h>
#include <simple3d/graphics/camera.h>
#include <simple3d/graphics/scene.h>
#include <simple3d/graphics/shader.h>
#include <simple3d/graphics/view.h>
#include <simple3d/misc/error.h>

#include <glm/mat4x4.hpp>
#include <sstream>

namespace Simple3D {

View::View() {
  // FIXME(apachee): create framebuffer
}

View::~View() {
  // FIXME(apachee): destroy framebuffer
}

Error View::Draw(const Scene& scene) {
  // FIXME(apachee): bind framebuffer
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  const glm::vec3& bg_color = scene.background_color;
  glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.0f);

  auto& cam = scene.active_camera_;
  auto proj = cam->GetProj();
  auto view = cam->GetView();
  auto view_pos = cam->GetViewPos();
  auto& directional_lights = scene.directional_lights_;
  auto& ambient_light = scene.ambient_light;

  for (auto& [cell_type, cell] : scene.renderers_) {
    if (auto shader_ptr = cell.shader.lock(); shader_ptr) {
      shader_ptr->SetProj(proj);
      shader_ptr->SetView(view);
      shader_ptr->SetViewPos(view_pos);
      shader_ptr->SetDirectionalLights(directional_lights);
      shader_ptr->SetAmbientLight(ambient_light);

      shader_ptr->Use();
      for (auto& [renderer_type, renderer] : cell.renderers) {
        renderer->Draw(shader_ptr.get());
      }
    } else {
      std::ostringstream ostrm{};
      ostrm << "unitialized shader of type " << cell_type.name();
      return Error(ErrorType::kUnitializedShader, ostrm.str());
    }
  }

  glFlush();

  return Error::Ok();
}

}  // namespace Simple3D
