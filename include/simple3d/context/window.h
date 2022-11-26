/**
 * \file
 * \brief Window handling
*/

#ifndef INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_
#define INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_

#include <memory>

#include <GLFW/glfw3.h>

#include <simple3d/types.h>

namespace Simple3D {



class Window : public std::enable_shared_from_this<Window> {
 public:
  static std::shared_ptr<Window> Create(GLFWwindow* window);

  ~Window();

  void Destroy();  // destroy window, invalidate pointer

 private:
  Window() = default;
  explicit Window(GLFWwindow* window);

  GLFWwindow* window_{nullptr};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_
