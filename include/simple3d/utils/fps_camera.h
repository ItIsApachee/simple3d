#ifndef INCLUDE_SIMPLE3D_UTILS_FPS_CAMERA_H_
#define INCLUDE_SIMPLE3D_UTILS_FPS_CAMERA_H_

#include <memory>
#include <chrono>

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <simple3d/graphics/camera.h>
#include <simple3d/context/input.h>

namespace Simple3D {

struct FpsCameraConfig {
 public:
  GLFWwindow* window{nullptr};
  float sensitivity{1.0F};
  bool raw_motion_enabled{false};

  float movement_speed{15.0F};

  int forwards_key{GLFW_KEY_W};
  int backwards_key{GLFW_KEY_S};
  int left_key{GLFW_KEY_A};
  int right_key{GLFW_KEY_D};
  int up_key{GLFW_KEY_SPACE};
  int down_key{GLFW_KEY_LEFT_SHIFT};
};

class FpsCameraInputHandler : public IInputHandler, public IWindowInputHandler {
 public:
  FpsCameraInputHandler() = default;
  FpsCameraInputHandler(const FpsCameraConfig& cfg);
  ~FpsCameraInputHandler() override = default;

  // TODO(apachee): set prev_xpos, prev_ypos to curr pos
  void Enable(const std::shared_ptr<Camera>& camera);
  void Disable();

  void Update(const std::chrono::milliseconds& delta);

  // void KeyCallback(int key, 
  //     int scancode, int action, int mods) override;
  void CursorPosCallback(double xpos, double ypos) override;

  void WindowFocusCallback(int focused) override;
  void FramebufferSizeCallback(int width, int height) override;

 private:
  struct KeyStates {
    char forwards{0};
    char backwards{0};
    char left{0};
    char right{0};
    char up{0};
    char down{0};
  };

  KeyStates GetKeyStates();

  std::shared_ptr<Camera> camera_{};

  FpsCameraConfig cfg_{};

  bool focused_{false};
  double prev_xpos_{0.0f};
  double prev_ypos_{0.0f};

  int prev_cursor_mode{GLFW_CURSOR_NORMAL};
};

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_UTILS_FPS_CAMERA_H_