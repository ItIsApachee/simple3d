#include <iostream>
#include <memory>
#include <cmath>
#include <chrono>

#include <glad/gles2.h>

#include <imgui.h>

#include <simple3d/simple3d.h>
#include <simple3d/graphics/model.h>
#include <simple3d/graphics/camera.h>
#include <simple3d/graphics/models/cuboid.h>
#include <simple3d/graphics/internal/shader.h>
#include <simple3d/utils/fps_camera.h>
#include <simple3d/imgui/imgui.h>

int main() {
	auto app_init_error = Simple3D::App::Init();
    if (!app_init_error.IsOk()) {
        std::cerr << "initialization failed: " << app_init_error.description << std::endl;
        return -1;
    }

    Simple3D::ImGui::CreateContext();

    std::cout << "init succ" << std::endl;
    
	Simple3D::View view{};
	Simple3D::Scene scene{};

    auto camera = std::make_shared<Simple3D::Camera>();
    scene.SetCamera(camera);

    Simple3D::FpsCameraConfig cfg{};
    cfg.raw_motion_enabled = true;
    cfg.window = Simple3D::App::GetGLFWwindow();
    auto cam_handler = std::make_shared<Simple3D::FpsCameraInputHandler>(cfg);
    Simple3D::App::EnableInputHandler(cam_handler);
    Simple3D::App::EnableWindowInputHandler(cam_handler);
    // cam_handler->Enable(camera);

    std::vector<Simple3D::Model<Simple3D::Cuboid>> cubes;
    int v = 1;
    float stride = 1.5f;
    for (int i = -v; i <= v; i++) {
        for (int j = -v; j <= v; j++) {
            for (int k = -v; k <= v; k++) {
                cubes.push_back(scene.Create<Simple3D::Cuboid>(stride*j, stride*i, stride*k));
            }
        }
        Simple3D::App::PollEvents();
        std::cout << (i + v + 1) << " / " << (2*v+1) << std::endl;
    }
    std::cout << "cubes: " << cubes.size() << std::endl;

	int cnt = 0;
    float angle = 0.0f;
    float dist = 10.0f;
    auto start = std::chrono::high_resolution_clock::now();
    auto prev = start;
	while (!Simple3D::App::ShouldClose()) {
		Simple3D::App::PollEvents();

        auto now = std::chrono::high_resolution_clock::now();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - prev);
        prev = now;
        cam_handler->Update(elapsed);

        float shift_amp = 0.5;
        float shift = shift_amp * sin(angle);
        for (int i = 9; i < 18; i++) {
            cubes[i]->y = shift;
            if (i % 2) {
                cubes[i]->y = -cubes[i]->y;
            }
        }
        angle = millis.count() / 5000.0f;

		view.Draw(scene);

        Simple3D::ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        Simple3D::ImGui::Render();

        Simple3D::App::SwapBuffers();
	}

    Simple3D::ImGui::DestroyContext();

    Simple3D::App::Destroy();
    return 0;
}