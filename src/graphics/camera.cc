#include <simple3d/graphics/camera.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Simple3D {



glm::mat4 ICamera::GetProj() {
  return glm::mat4(1.0f);
}

glm::mat4 ICamera::GetView() {
  return glm::mat4(1.0f);
}

glm::vec3 ICamera::GetViewPos() {
  return glm::vec3(0.0f);
}

glm::mat4 Camera::GetView() {
  glm::mat4 view(1.0f);
  view = glm::rotate(view, roll, glm::vec3(0.0f, 0.0f, 1.0f));
  view = glm::rotate(view, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::rotate(view, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
  view = glm::translate(view, -pos);
  return view;
}

glm::mat4 Camera::GetProj() {
  glm::mat4 proj = glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, 1000.0f);
  return proj;
}

glm::vec3 Camera::GetViewPos() {
  return pos;
}



}  // namespace Simple3D
