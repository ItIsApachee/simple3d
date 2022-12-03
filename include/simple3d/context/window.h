/**
 * \file
 * \brief Window handling
*/

#ifndef INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_
#define INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_

#include <vector>
#include <memory>

#include <GLFW/glfw3.h>

#include <simple3d/types.h>
#include <simple3d/context/input.h>

namespace Simple3D {



class Window : public std::enable_shared_from_this<Window> {
 public:
  static std::shared_ptr<Window> Create(GLFWwindow* window);

  ~Window();

  void AddInputHandler(std::shared_ptr<IInputHandler> input_handler);
  void AddWindowInputHandler(
    std::shared_ptr<IWindowInputHandler> window_input_handler);

  void SwapBuffers();
  void Destroy();  // destroy window, invalidate pointer

 private:
  Window() = default;
  explicit Window(GLFWwindow* window);

  GLFWwindow* window_{nullptr};
  std::vector<std::shared_ptr<IInputHandler>> input_handlers_{};
  std::vector<std::shared_ptr<IWindowInputHandler>> window_input_handlers_{};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_
