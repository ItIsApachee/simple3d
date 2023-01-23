#include <glad/gles2.h>
#include <simple3d/graphics/camera.h>
#include <simple3d/graphics/scene.h>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <utility>

namespace Simple3D {

Scene::Scene()
    : renderers_{},
      active_camera_{std::make_shared<ICamera>()},
      directional_lights_{} {}

void Scene::SetCamera(std::shared_ptr<ICamera> camera) {
  if (camera) {
    active_camera_ = camera;
  } else {
    active_camera_ = std::shared_ptr<ICamera>(new ICamera());
  }
}

Scene::Scene(Scene&&other)
    : ambient_light{other.ambient_light},
      background_color{other.background_color},
      renderers_{std::move(other.renderers_)},
      active_camera_{std::move(other.active_camera_)},
      directional_lights_{std::move(other.directional_lights_)} {}

Scene& Scene::operator=(Scene&& other) {
  if (&other == this)
    return *this;
  
  ambient_light = other.ambient_light;
  background_color = other.background_color;
  renderers_ = std::move(other.renderers_);
  active_camera_ = std::move(other.active_camera_);
  directional_lights_ = std::move(other.directional_lights_);

  return *this;
}

void Scene::AddDirectionalLight(
    const std::shared_ptr<DirectionalLight>& dir_light) {
  directional_lights_.insert(dir_light);
}

void Scene::RemoveDirectionalLight(
    const std::shared_ptr<DirectionalLight>& dir_light) {
  directional_lights_.erase(dir_light);
}

// void Scene::SetAmbientLight(const glm::vec3& light) { ambient_light = light;
// }
//
// void Scene::SetBackgroundColor(const glm::vec3& bg_color) {
//   background_color = bg_color;
// }

}  // namespace Simple3D
