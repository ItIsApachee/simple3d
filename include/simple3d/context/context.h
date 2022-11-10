#ifndef SIMPLE3D_CONTEXT_CONTEXT_H
#define SIMPLE3D_CONTEXT_CONTEXT_H

#include <unordered_set>
#include <string>
#include <memory>

#include <GLFW/glfw3.h>

#include <simple3d/misc/error.h>

namespace Simple3D {

class MainLoop;
class Window;
class WindowBuilder;

// Singleton for managing windows, inputs, etc.
// TODO: add customization, e.g. use native GLES
class MainLoop {
public:
    static MainLoop& GetInstance();

    MainLoop(const MainLoop&) = delete;
    MainLoop(MainLoop&&) = delete;
    MainLoop& operator=(const MainLoop&) = delete;
    MainLoop& operator=(MainLoop&&) = delete;

    ~MainLoop() = default;

    Error Init();
    // only in main thread
    Error Start();
    void Destroy();

    void AddWindow(std::shared_ptr<Window> window);
private:
    explicit MainLoop() = default;

    std::unordered_set<std::shared_ptr<Window>> windows_;
};

// class representing window, and its GLES context
// FIXME: conceptually it should inherit std::enable_shared_from_this<Window>
class Window {
public:
    friend WindowBuilder;

    GLFWwindow* GetGLFWwindow() const;

    // TODO: define proper destructor
    ~Window() = default;
private:
    explicit Window(GLFWwindow* glfw_window): glfw_window_{glfw_window} {}
    Window(const Window&) = delete;
    Window(Window&&) = default;

    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = default;

    GLFWwindow* glfw_window_;
};

class WindowBuilder {
public:
    explicit WindowBuilder() = default;
    WindowBuilder(const WindowBuilder&) = default;
    WindowBuilder(WindowBuilder&&) = default;

    WindowBuilder& operator=(const WindowBuilder&) = default;
    WindowBuilder& operator=(WindowBuilder&&) = default;

    ~WindowBuilder() = default;

    std::shared_ptr<Window> Build();
    WindowBuilder& Title(std::string);
private:
    std::string title_{};
};

}

#endif