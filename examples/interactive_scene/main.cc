#include <iostream>

#include <glad/gles2.h>

#include <simple3d/simple3d.h>
#include <simple3d/graphics/model.h>
#include <simple3d/graphics/models/cuboid.h>
#include <simple3d/graphics/internal/shader.h>
// #include <iostream>
// #include <thread>
// #include <chrono>
// #include <vector>
// #include <cmath>
// #include <windows.h>

// #define GLAD_GLES2_IMPLEMENTATION
// #include <glad/gles2.h>

// #include <GLFW/glfw3.h>

// void error_callback(int error, const char* description) {
//     std::cerr << "glfw error: " << error << " with message \"" << description << "\"" << std::endl;
// }

// void render(GLFWwindow* window) {
//     constexpr int FPS_SAMPLES = 200;
//     auto start = std::chrono::high_resolution_clock::now();
//     std::vector<decltype(start)> fps_samples(FPS_SAMPLES, start);

//     glfwMakeContextCurrent(window);

//     GladGLES2Context gl;
//     int version = gladLoadGLES2Context(&gl, glfwGetProcAddress);
//     if (version == 0) {
//         std::cerr << "glad: loading gles2 failed" << std::endl;
//         return;
//     }
//     std::cout << "successfuly loaded GLES2 v" << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

//     const float PI = acos(-1.0);

//     // FIXME: this function may only be called from main thread
//     // glfwGetFramebufferSize(window, &display_w, &display_h);
//     gl.Viewport(0, 0, 2560, 1440);

//     float v = 0;
//     int cnt = 0;
//     // FIXME: should syncronize calls to this function (data race)
//     while (!glfwWindowShouldClose(window)) {
//         cnt = (cnt + 1) % FPS_SAMPLES;
//         auto now = std::chrono::high_resolution_clock::now();

//         int took = (now - fps_samples[cnt]).count();
//         fps_samples[cnt] = now;
//         std::cout << "FPS: " << (FPS_SAMPLES / (took / 1e9)) << std::endl;

//         auto elapsed = (now - start).count();
//         v = elapsed / 1e9;

//         gl.ClearColor((sin(v*2*PI)+1.)/2, 0.0f, 0.0f, 1.0f);
//         gl.Clear(GL_COLOR_BUFFER_BIT);
//         // gl.Flush();
//         glfwSwapBuffers(window);
//     }
// }

int main() {
	using namespace Simple3D;

	Context::Init();
	View view{};
	Scene scene{};
	// auto test1 = scene.Create<TestPrimitive>();
	// auto test2 = scene.Create<TestPrimitive>(0);
	// auto test3 = scene.Create<TestPrimitive, TestPrimitiveRenderer>();
	// auto test4 = scene.Create<TestPrimitive, TestPrimitiveRenderer>(0);


    std::vector<Model<Cuboid>> cubes;
    int v = 5;
    for (int i = -v; i <= v; i++) {
        for (int j = -v; j <= v; j++) {
            cubes.push_back(scene.Create<Cuboid>(i*2, j*2, -30.0f));
        }
    }
	// auto cube = scene.Create<Cuboid, CuboidRenderer>(0.f, 0.f, -10.0f);
    // cube->r = cube->g = cube->b = 1.0f;

	// Error err;
	// Internal::Shader Shader = Internal::ShaderBuilder()
	// 		.FragmentShaderSource(Internal::kFragmentShader)
	// 		.VertexShaderSource(Internal::kVertexShader)
	// 		.Build(&err);

	int cnt = 0;
	auto window = Context::GetWindow();
	while (true) {
		Context::PollEvents();
		// test2->id = ++cnt;
		// std::cout << "dbg: " << test1->id << std::endl;
		view.Draw(scene);
		window->SwapBuffers();
	}


    // auto init_error = MainLoop::GetInstance().Init();
    // if (!init_error.IsOk()) {
    //     std::cerr << "init_error: " << init_error.description << std::endl;
    //     return -1;
    // }

    // auto window = WindowBuilder().Title("test").Build();
    // auto scene = Scene::Create();
    // scene->SetContext(window);

    // auto loop_error = MainLoop::GetInstance().Start();
    // if (!loop_error.IsOk()) {
    //     std::cerr << "loop_error: " << loop_error.description << std::endl;
    // }

    // MainLoop::GetInstance().Destroy();


    // if (!glfwInit()) {
    //     std::cerr << "glfw initialization failed" << std::endl;
    //     return 1;
    // }
    // glfwSetErrorCallback(error_callback);

    // glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

    // glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // // doesn't work with ANGLE for some reason
    // // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE); // disable vsync

    // GLFWwindow* window = glfwCreateWindow(500, 500, "My title", nullptr, nullptr);
    // if (!window) {
    //     std::cerr << "window creation failed" << std::endl;
    //     return 2;
    // }

    // glfwSetWindowAspectRatio(window, 16, 9);

    // std::thread render_thread(render, window);

    // while (!glfwWindowShouldClose(window)) {
    //     glfwWaitEvents();
    // }

    // render_thread.join();

    // glfwDestroyWindow(window);
    // glfwTerminate();

    return 0;
}