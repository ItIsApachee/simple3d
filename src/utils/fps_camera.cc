#include <GLFW/glfw3.h>
#include <simple3d/context/context.h>
#include <simple3d/utils/fps_camera.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace Simple3D {

FpsCameraInputHandler::FpsCameraInputHandler()
    : FpsCameraInputHandler{FpsCameraConfig{}} {}

FpsCameraInputHandler::FpsCameraInputHandler(const FpsCameraConfig& cfg)
    : cfg{cfg}, window_{App::GetGLFWwindow()} {
  focused_ = glfwGetWindowAttrib(window_, GLFW_FOCUSED) == GLFW_TRUE;
}

void FpsCameraInputHandler::Enable(const std::shared_ptr<Camera>& camera) {
  glfwGetCursorPos(window_, &prev_xpos_, &prev_ypos_);
  // TODO(apachee): disable cursor & raw mouse motion
  prev_cursor_mode = glfwGetInputMode(window_, GLFW_CURSOR);
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (cfg.raw_motion_enabled && glfwRawMouseMotionSupported()) {
    glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }

  camera_ = camera;
}
void FpsCameraInputHandler::Disable() {
  if (glfwGetInputMode(window_, GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE) {
    glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
  }
  glfwSetInputMode(window_, GLFW_CURSOR, prev_cursor_mode);
  camera_ = {};
}

bool FpsCameraInputHandler::IsEnabled() { return bool{camera_}; }

FpsCameraInputHandler::KeyStates FpsCameraInputHandler::GetKeyStates() {
  FpsCameraInputHandler::KeyStates key_states{};

  auto check = [this](int key) {
    return glfwGetKey(window_, key) == GLFW_PRESS;
  };

  key_states.forwards = check(cfg.forwards_key);
  key_states.backwards = check(cfg.backwards_key);
  key_states.left = check(cfg.left_key);
  key_states.right = check(cfg.right_key);
  key_states.up = check(cfg.up_key);
  key_states.down = check(cfg.down_key);
  if (cfg.escape_key) key_states.escape = check(cfg.escape_key.value());

  return key_states;
}

bool FpsCameraInputHandler::Update(const std::chrono::milliseconds& delta) {
  if (camera_ && focused_) {
    bool raw_motion_enabled =
        glfwGetInputMode(window_, GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE;
    if (raw_motion_enabled != cfg.raw_motion_enabled) {
      glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION,
                       cfg.raw_motion_enabled ? GLFW_TRUE : GLFW_FALSE);
    }

    constexpr float kMillisPerSecond = 1000.0F;
    float mult = cfg.movement_speed * delta.count() / kMillisPerSecond;

    auto key_states = GetKeyStates();
    if (key_states.escape) {
      Disable();
      return true;
    }

    glm::vec3 cam_front{-glm::sin(-camera_->yaw), 0.0F,
                        -glm::cos(-camera_->yaw)};
    float front_mult = mult * (key_states.forwards - key_states.backwards);
    camera_->pos += front_mult * cam_front;

    glm::vec3 cam_right{glm::cos(-camera_->yaw), 0.0F,
                        -glm::sin(-camera_->yaw)};
    float right_mult = mult * (key_states.right - key_states.left);
    camera_->pos += right_mult * cam_right;

    glm::vec3 cam_up{0.0F, 1.0F, 0.0F};
    float up_mult = mult * (key_states.up - key_states.down);
    camera_->pos += up_mult * cam_up;
    return false;
  }
  return true;
}

void FpsCameraInputHandler::CursorPosCallback(double xpos, double ypos) {
  if (camera_ && focused_) {
    const double pixels_to_radians = glm::radians(0.25f);

    float xdiff = xpos - prev_xpos_;
    xdiff *= pixels_to_radians;
    using yaw_t = decltype(camera_->yaw);
    camera_->yaw += cfg.sensitivity * xdiff;
    camera_->yaw = std::fmod(camera_->yaw, glm::two_pi<yaw_t>());

    float ydiff = ypos - prev_ypos_;
    ydiff *= pixels_to_radians;
    using pitch_t = decltype(camera_->pitch);
    camera_->pitch += cfg.sensitivity * ydiff;
    camera_->pitch = std::clamp<pitch_t>(
        camera_->pitch, -glm::half_pi<pitch_t>(), glm::half_pi<pitch_t>());

    prev_xpos_ = xpos;
    prev_ypos_ = ypos;
  }
}

void FpsCameraInputHandler::WindowFocusCallback(int focused) {
  focused_ = focused == GLFW_TRUE;
}
void FpsCameraInputHandler::FramebufferSizeCallback(int width, int height) {
  if (camera_)
    camera_->aspect_ratio =
        static_cast<float>(width) / static_cast<float>(height);
}

}  // namespace Simple3D
