#include <iostream>
#include <memory>
#include <cmath>
#include <unordered_set>
#include <chrono>

#include <glad/gles2.h>

#include <imgui.h>

#include <simple3d/simple3d.h>
#include <simple3d/context/input.h>
#include <simple3d/graphics/model.h>
#include <simple3d/graphics/camera.h>
#include <simple3d/graphics/light.h>
#include <simple3d/graphics/models/cuboid.h>
#include <simple3d/utils/fps_camera.h>
#include <simple3d/imgui/imgui.h>

class FocusFpsCam : public Simple3D::IInputHandler {
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
 private:
  std::shared_ptr<Simple3D::FpsCameraInputHandler> fps_cam_handler{};
  std::shared_ptr<Simple3D::Camera> cam{};
};

int main() {
	auto app_init_error = Simple3D::App::Init();
  if (!app_init_error.IsOk()) {
    std::cerr << "initialization failed: " << app_init_error.description << std::endl;
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

  scene.SetAmbientLight(glm::vec3(0.5f, 0.5f, 0.5f));
  using light_t = Simple3D::DirectionalLight;
  std::unordered_set<std::shared_ptr<light_t>> lights = {
    std::shared_ptr<light_t>(new light_t{
      glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f)),
      glm::vec3(1.0f, 0.0f, 0.0f) * 0.3f,
      glm::vec3(1.0f, 0.0f, 0.0f) * 0.2f}),
    std::shared_ptr<light_t>(new light_t{
      glm::normalize(glm::vec3(0.0f, -1.0f, 1.0f)),
      glm::vec3(0.0f, 1.0f, 0.0f) * 0.3f,
      glm::vec3(0.0f, 1.0f, 0.0f) * 0.2f}),
    std::shared_ptr<light_t>(new light_t{
      glm::normalize(glm::vec3(-1.0f, -1.0f, 0.0f)),
      glm::vec3(0.0f, 0.0f, 1.0f) * 0.3f,
      glm::vec3(0.0f, 0.0f, 1.0f) * 0.2f}),
    std::shared_ptr<light_t>(new light_t{
      glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f)),
      glm::vec3(1.0f, 0.3f, 0.4f) * 0.3f,
      glm::vec3(1.0f, 0.3f, 0.4f) * 0.2f}),
  };
  for (auto &light : lights)
    scene.AddDirectionalLight(light);

  auto camera = std::make_shared<Simple3D::Camera>();
  scene.SetCamera(camera);

  Simple3D::FpsCameraConfig default_cfg{};
  default_cfg.raw_motion_enabled = true;
  default_cfg.escape_key = GLFW_KEY_ESCAPE;
  auto cam_handler = std::make_shared<Simple3D::FpsCameraInputHandler>(default_cfg);
  // Simple3D::App::EnableInputHandler(cam_handler);
  // Simple3D::App::EnableWindowInputHandler(cam_handler);
  imgui_handler->EnableInputHandler(cam_handler);
  imgui_handler->EnableWindowInputHandler(cam_handler);
  cam_handler->Enable(camera);

  imgui_handler->EnableInputHandler(std::make_shared<FocusFpsCam>(
    cam_handler, camera));

  std::vector<Simple3D::Model<Simple3D::Cuboid>> cubes;
  int v = 10;
  float stride = 1.5f;
  for (int i = -v; i <= v; i++) {
    for (int j = -v; j <= v; j++) {
      for (int k = -v; k <= v; k++) {
        cubes.push_back(scene.Create<Simple3D::Cuboid>(
          glm::vec3(stride*j, stride*i, stride*k)));
        cubes.back()->diffuse_color = glm::vec3(0.55f);
        cubes.back()->specular_color = glm::vec3(0.7f);
        cubes.back()->shininess = 0.25f;
      }
    }
    Simple3D::App::PollEvents();
    std::cout << (i + v + 1) << " / " << (2*v+1) << std::endl;
  }
  std::cout << "cubes: " << cubes.size() << std::endl;

  int cnt = 0;
  // float angle = 0.0f;
  float dist = 10.0f;
  auto start = std::chrono::high_resolution_clock::now();
  auto prev = start;
  while (!Simple3D::App::ShouldClose()) {
    Simple3D::App::PollEvents();

    auto now = std::chrono::high_resolution_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - prev);
    prev = now;

    if (cam_handler->Update(elapsed)) {
        imgui_handler->ToggleInputs(true);
    }

    // float shift_amp = 0.5;
    // float shift = shift_amp * sin(angle);
    // for (int i = 9; i < 18; i++) {
    //     cubes[i]->pos.y = shift;
    //     if (i % 2) {
    //         cubes[i]->pos.y = -cubes[i]->pos.y;
    //     }
    // }
    // angle = millis.count() / 5000.0f;

    view.Draw(scene);

    Simple3D::ImGui::NewFrame();

    ImGui::ShowDemoWindow();

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
    if (ImGui::Button("Reset to default"))
      cfg = default_cfg;
    ImGui::End();

    Simple3D::ImGui::Render();

    Simple3D::App::SwapBuffers();
  }

  Simple3D::ImGui::DestroyContext();
  Simple3D::App::Destroy();

  return 0;
}