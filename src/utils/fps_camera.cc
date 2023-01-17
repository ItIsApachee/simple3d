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
  prev_cursor_mode = glfwGetInputMode(cfg_.window, GLFW_CURSOR);
  glfwSetInputMode(cfg_.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (cfg_.raw_motion_enabled && glfwRawMouseMotionSupported()) {
    glfwSetInputMode(cfg_.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }

  camera_ = camera;
}
void FpsCameraInputHandler::Disable() {
  if (glfwGetInputMode(cfg_.window, GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE) {
    glfwSetInputMode(cfg_.window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
  }
  glfwSetInputMode(cfg_.window, GLFW_CURSOR, prev_cursor_mode);
  camera_ = {};
}

FpsCameraInputHandler::KeyStates FpsCameraInputHandler::GetKeyStates() {
  FpsCameraInputHandler::KeyStates key_states{};
  
  auto check = [this](int key) {
    return glfwGetKey(cfg_.window, key) == GLFW_PRESS;
  };

  key_states.forwards = check(cfg_.forwards_key);
  key_states.backwards = check(cfg_.backwards_key);
  key_states.left = check(cfg_.left_key);
  key_states.right = check(cfg_.right_key);
  key_states.up = check(cfg_.up_key);
  key_states.down = check(cfg_.down_key);

  return key_states;
}

void FpsCameraInputHandler::Update(const std::chrono::milliseconds& delta) {
  if (!camera_ && !focused_)
    return;
  
  constexpr float kMillisPerSecond = 1000.0F;
  float mult = cfg_.movement_speed * (float)delta.count() / kMillisPerSecond;

  auto key_states = GetKeyStates();

  glm::vec3 cam_front{-glm::sin(-camera_->yaw), 0.0F, -glm::cos(-camera_->yaw)};
  float front_mult = mult * (key_states.forwards - key_states.backwards);
  camera_->pos += front_mult * cam_front;

  glm::vec3 cam_right{glm::cos(-camera_->yaw), 0.0F, -glm::sin(-camera_->yaw)};
  float right_mult = mult * (key_states.right - key_states.left);
  camera_->pos += right_mult * cam_right;

  glm::vec3 cam_up{0.0F, 1.0F, 0.0F};
  float up_mult = mult * (key_states.up - key_states.down);
  camera_->pos += up_mult * cam_up;
}

FpsCameraInputHandler::FpsCameraInputHandler(const FpsCameraConfig& cfg)
    : cfg_{cfg} {
  focused_ = glfwGetWindowAttrib(cfg_.window, GLFW_FOCUSED) == GLFW_TRUE;
}

void FpsCameraInputHandler::CursorPosCallback(double xpos, double ypos) {
  if (!camera_ || !focused_)
    return;

  const double pixels_to_radians = glm::radians(0.25f);

  float xdiff = (float)(xpos - prev_xpos_);
  xdiff *= pixels_to_radians;
  using yaw_t = decltype(camera_->yaw);
  camera_->yaw += cfg_.sensitivity * xdiff;
  camera_->yaw = std::fmod(camera_->yaw, glm::two_pi<yaw_t>());

  float ydiff = (float)(ypos - prev_ypos_);
  ydiff *= pixels_to_radians;
  using pitch_t = decltype(camera_->pitch);
  camera_->pitch += cfg_.sensitivity * ydiff;
  camera_->pitch = std::clamp<pitch_t>(
      camera_->pitch, -glm::half_pi<pitch_t>(), glm::half_pi<pitch_t>());

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
