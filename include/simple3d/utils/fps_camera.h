#ifndef INCLUDE_SIMPLE3D_UTILS_FPS_CAMERA_H_
#define INCLUDE_SIMPLE3D_UTILS_FPS_CAMERA_H_

#include <GLFW/glfw3.h>
#include <simple3d/context/input.h>
#include <simple3d/graphics/camera.h>

#include <chrono>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <optional>

namespace Simple3D {

struct FpsCameraConfig {
 public:
  float sensitivity{1.0F};
  bool raw_motion_enabled{false};

  float movement_speed{15.0F};

  int forwards_key{GLFW_KEY_W};
  int backwards_key{GLFW_KEY_S};
  int left_key{GLFW_KEY_A};
  int right_key{GLFW_KEY_D};
  int up_key{GLFW_KEY_SPACE};
  int down_key{GLFW_KEY_LEFT_SHIFT};

  std::optional<int> escape_key{};
};

class FpsCameraInputHandler : public IInputHandler, public IWindowInputHandler {
 public:
  FpsCameraInputHandler();
  explicit FpsCameraInputHandler(const FpsCameraConfig& cfg);
  ~FpsCameraInputHandler() override = default;

  void Enable(const std::shared_ptr<Camera>&);
  void Disable();

  bool IsEnabled();
  bool Update(const std::chrono::milliseconds& delta);

  void CursorPosCallback(double xpos, double ypos) override;

  void WindowFocusCallback(int focused) override;
  void FramebufferSizeCallback(int width, int height) override;

  FpsCameraConfig cfg{};

 private:
  struct KeyStates {
    char forwards{0};
    char backwards{0};
    char left{0};
    char right{0};
    char up{0};
    char down{0};
    bool escape{0};
  };

  KeyStates GetKeyStates();

  GLFWwindow* window_{nullptr};

  std::shared_ptr<Camera> camera_{};

  bool focused_{false};
  double prev_xpos_{0.0f};
  double prev_ypos_{0.0f};

  int prev_cursor_mode{GLFW_CURSOR_NORMAL};
};

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_UTILS_FPS_CAMERA_H_
