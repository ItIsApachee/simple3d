#include <simple3d/graphics/camera.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Simple3D {



glm::mat4 ICamera::GetProj() {
  // glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 1000.0f);
  // return projection;
  return glm::mat4(1.0f);
}

glm::mat4 ICamera::GetView() {
  // glm::mat4 view(1.0f);
  // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -100.0f));
  // // view = glm::rotate(view, elapsed.count() / 25000.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  // // view = glm::rotate(view, elapsed.count() / 4000.0f, glm::vec3(0.0f, 1.0f, 0.0f));

  // return view;
  return glm::mat4(1.0f);
}



}  // namespace Simple3D
