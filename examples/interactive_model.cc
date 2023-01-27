#include <glad/gles2.h>
#include <imgui.h>
#include <simple3d/context/input.h>
#include <simple3d/graphics/camera.h>
#include <simple3d/graphics/light.h>
#include <simple3d/graphics/model_handle.h>
#include <simple3d/graphics/models/model.h>
#include <simple3d/imgui/imgui.h>
#include <simple3d/simple3d.h>
#include <simple3d/utils/fps_camera.h>

#include <chrono>
#include <cmath>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class FocusFpsCam
    : public Simple3D::IInputHandler,
      public Simple3D::IWindowInputHandler {
 public:
  explicit FocusFpsCam(
      const std::shared_ptr<Simple3D::FpsCameraInputHandler>& v_,
      const std::shared_ptr<Simple3D::Camera>& c_)
      : fps_cam_handler{v_}, cam{c_} {}

  void MouseButtonCallback(int button, int action, int mods) override {
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
      if (fps_cam_handler && !fps_cam_handler->IsEnabled()) {
        fps_cam_handler->Enable(cam);
      }
    }
  }

  void WindowFocusCallback(int focused) override {
    if (focused == GLFW_FALSE) {
      if (fps_cam_handler && fps_cam_handler->IsEnabled()) {
        fps_cam_handler->Disable();
      }
    }
  }

 private:
  std::shared_ptr<Simple3D::FpsCameraInputHandler> fps_cam_handler{};
  std::shared_ptr<Simple3D::Camera> cam{};
};

int main() {
  auto app_init_error = Simple3D::App::Init();
  if (!app_init_error.IsOk()) {
    std::cerr << "initialization failed: " << app_init_error.description
              << std::endl;
    return -1;
  }

  Simple3D::ImGui::CreateContext();

  auto imgui_handler = std::make_shared<Simple3D::ImGui::InputHandler>();
  Simple3D::App::EnableInputHandler(imgui_handler);
  Simple3D::App::EnableWindowInputHandler(imgui_handler);
  imgui_handler->ToggleInputs(false);

  std::cout << "init succ" << std::endl;

  Simple3D::View view{};
  Simple3D::Scene scene{};

  scene.background_color = glm::vec3(1.0f);
  scene.ambient_light = glm::vec3(0.5f);

  using light_t = Simple3D::DirectionalLight;
  glm::vec3 white(1.0f);
  std::cout << "white: " << white.x << white.y << white.z << std::endl;

  std::unordered_set<std::shared_ptr<light_t>> lights;
  {
    std::vector<std::pair<glm::vec3, glm::vec3>> light_cfg = {
        {glm::vec3(0.0f, -1.0f, 0.0f), white * 0.9f},
        {glm::vec3(1.0f, 0.0f, 0.0f), white * 0.8f},
        {glm::vec3(0.0f, 0.0f, 1.0f), white * 0.7f},
        {glm::vec3(-1.0f, 0.0f, 0.0f), white * 0.55f},
        {glm::vec3(0.0f, 0.0f, -1.0f), white * 0.4f}};
    for (const auto& [dir, base_color] : light_cfg) {
      lights.emplace(new light_t{dir, base_color * 0.5f, base_color * 0.3f});
    }
  }
  for (const auto& light : lights) {
    scene.AddDirectionalLight(light);
  }

  auto camera = std::make_shared<Simple3D::Camera>();
  camera->pos = {-5.0f, 10.0f, -5.0f};
  camera->pitch = glm::radians(45.0f);
  camera->yaw = glm::radians(135.0f);
  scene.SetCamera(camera);

  Simple3D::FpsCameraConfig default_cfg{};
  default_cfg.raw_motion_enabled = true;
  default_cfg.escape_key = GLFW_KEY_ESCAPE;
  auto cam_handler =
      std::make_shared<Simple3D::FpsCameraInputHandler>(default_cfg);
  imgui_handler->EnableInputHandler(cam_handler);
  imgui_handler->EnableWindowInputHandler(cam_handler);
  cam_handler->Enable(camera);

  {
    auto focus_fps_cam = std::make_shared<FocusFpsCam>(cam_handler, camera);
    imgui_handler->EnableInputHandler(focus_fps_cam);
    imgui_handler->EnableWindowInputHandler(focus_fps_cam);
  }

  auto minecraft_cube_model =
      Simple3D::Model::Load("Minecraft_Grass_Block_OBJ/Grass_Block.obj");

  auto minecraft_cube =
      scene.Create<Simple3D::ModelInstance>(minecraft_cube_model);

  auto start = std::chrono::high_resolution_clock::now();
  auto prev = start;
  while (!Simple3D::App::ShouldClose()) {
    Simple3D::App::PollEvents();

    auto now = std::chrono::high_resolution_clock::now();

    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - prev);
    prev = now;

    if (cam_handler->Update(elapsed)) {
      imgui_handler->ToggleInputs(true);
    }

    float amplitude = 2.0f;
    auto elapsed_from_start =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
    minecraft_cube->pos.y = 2.0f * glm::sin(static_cast<float>(elapsed_from_start) / 1000.0f);

    view.Draw(scene);

    Simple3D::ImGui::NewFrame();

    ImGui::Begin("Scene controls");
    ImGui::PushItemWidth(150.0f);
    ImGui::ColorPicker3("background color",
                        glm::value_ptr(scene.background_color));
    ImGui::ColorPicker3("ambient light",
                        glm::value_ptr(scene.ambient_light));
    ImGui::PopItemWidth();
    ImGui::End();

    ImGui::Begin("Position");
    static_assert(std::is_same_v<decltype(camera->pos.x), float>);
    ImGui::InputFloat("x", &camera->pos.x);
    ImGui::InputFloat("y", &camera->pos.y);
    ImGui::InputFloat("z", &camera->pos.z);
    ImGui::End();

    ImGui::Begin("Camera controls");
    auto& cfg = cam_handler->cfg;
    ImGui::SliderFloat("Sensitivity", &cfg.sensitivity, 0.1f, 3.0f);
    ImGui::SliderFloat("Movement speed", &cfg.movement_speed, 1.0f, 100.0f);
    if (ImGui::RadioButton("Raw inputs", cfg.raw_motion_enabled)) {
      cfg.raw_motion_enabled = !cfg.raw_motion_enabled;
    }
    if (ImGui::Button("Reset to default")) cfg = default_cfg;
    ImGui::End();

    Simple3D::ImGui::Render();

    Simple3D::App::SwapBuffers();
  }

  Simple3D::ImGui::DestroyContext();
  Simple3D::App::Destroy();

  return 0;
}
