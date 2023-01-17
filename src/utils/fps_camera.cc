#include <simple3d/utils/fps_camera.h>

#include <chrono>
#include <memory>
#include <algorithm>
#include <cmath>

#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <GLFW/glfw3.h>

namespace Simple3D {

void FpsCameraInputHandler::Enable(const std::shared_ptr<Camera>& camera) {
  glfwGetCursorPos(cfg_.window, &prev_xpos_, &prev_ypos_);
  // TODO(apachee): disable cursor & raw mouse motion
  glfwSetInputMode(cfg_.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  key_states_ = KeyStates{};
  camera_ = camera;
}
void FpsCameraInputHandler::Disable() {
  glfwSetInputMode(cfg_.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  camera_ = {};
}

void FpsCameraInputHandler::Update(const std::chrono::milliseconds& delta) {
  if (!camera_ && !focused_)
    return;
  
  constexpr float kMillisPerSecond = 1000.0F;
  float mult = cfg_.movement_speed * (float)delta.count() / kMillisPerSecond;

  glm::vec3 cam_front{-glm::sin(-camera_->yaw), 0.0F, -glm::cos(-camera_->yaw)};
  float front_mult = mult * (key_states_.forwards - key_states_.backwards);
  camera_->pos += front_mult * cam_front;

  glm::vec3 cam_right{glm::cos(-camera_->yaw), 0.0F, -glm::sin(-camera_->yaw)};
  float right_mult = mult * (key_states_.right - key_states_.left);
  camera_->pos += right_mult * cam_right;

  // glm::vec3 cam_up{0.0F, 1.0F, 0.0F};
  // camera_->pos += (int)vertical_ * cfg_.movement_speed * (float)delta.count() * cam_right;
}

FpsCameraInputHandler::FpsCameraInputHandler(const FpsCameraConfig& cfg)
    : cfg_{cfg} {
  focused_ = glfwGetWindowAttrib(cfg_.window, GLFW_FOCUSED) == GLFW_TRUE;
}

void FpsCameraInputHandler::KeyCallback(int key, 
    int scancode, int action, int mods) {
  if (!camera_)
    return;
  
  if (action != GLFW_PRESS && action != GLFW_RELEASE)
    return;
  char val = action == GLFW_PRESS;

  if (key == cfg_.forwards_key) {
    key_states_.forwards = val;
  } else if (key == cfg_.backwards_key) {
    key_states_.backwards = val;
  } else if (key == cfg_.left_key) {
    key_states_.left = val;
  } else if (key == cfg_.right_key) {
    key_states_.right = val;
  }
}
void FpsCameraInputHandler::CursorPosCallback(double xpos, double ypos) {
  if (!camera_ || !focused_)
    return;

  const double pixels_to_radians = glm::radians(0.25f);

  double xdiff = xpos - prev_xpos_;
  xdiff *= pixels_to_radians;
  camera_->yaw += cfg_.sensitivity * xdiff;
  camera_->yaw = std::fmod(camera_->yaw, glm::two_pi<double>());

  double ydiff = ypos - prev_ypos_;
  ydiff *= pixels_to_radians;
  camera_->pitch += cfg_.sensitivity * ydiff;
  camera_->pitch = std::clamp<decltype(camera_->pitch)>(
      camera_->pitch, -glm::half_pi<double>(), glm::half_pi<double>());

  prev_xpos_ = xpos;
  prev_ypos_ = ypos;
}

void FpsCameraInputHandler::WindowFocusCallback(int focused) {
  focused_ = focused == GLFW_TRUE;
}
void FpsCameraInputHandler::FramebufferSizeCallback(int width, int height) {
  if (!camera_)
    return;
  camera_->aspect_ratio = (float)width / (float)height;
}

}  // namespace Simple3D
