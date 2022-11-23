/**
 * \file
 * \brief Window handling
*/

#ifndef SIMPLE3D_CONTEXT_WINDOW_H
#define SIMPLE3D_CONTEXT_WINDOW_H

#include <memory>

#include <GLFW/glfw3.h>

#include <simple3d/types.h>

namespace Simple3D {



class Window : public std::enable_shared_from_this<Window> {
public:
    static std::shared_ptr<Window> Create(GLFWwindow* window);

    ~Window();

    void Destroy(); // destroy window, invalidate pointer
private:
    Window() = default;
    explicit Window(GLFWwindow* window);

    GLFWwindow* window_{nullptr};
};



}

#endif