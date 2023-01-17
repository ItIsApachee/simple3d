#include <simple3d/graphics/scene.h>

#include <cmath>

#include <iostream>
#include <memory>
#include <utility>

#include <glad/gles2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <simple3d/graphics/camera.h>

namespace Simple3D {



Scene::Scene()
    : renderers_{}, 
      active_camera_{new ICamera()}, 
      directional_lights_{} {
}

void Scene::SetCamera(std::shared_ptr<ICamera> camera) {
  if (camera) {
    active_camera_ = camera;
  } else {
    active_camera_ = std::shared_ptr<ICamera>(new ICamera());
  }
}

void Scene::AddDirectionalLight(const std::shared_ptr<DirectionalLight>& dir_light) {
  directional_lights_.insert(dir_light);
}

void Scene::RemoveDirectionalLight(const std::shared_ptr<DirectionalLight>& dir_light) {
  directional_lights_.erase(dir_light);
}

void Scene::SetAmbientLight(const glm::vec3& light) {
  ambient_light = light;
}



}  // namespace Simple3D
