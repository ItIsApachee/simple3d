/**
 * \file
 * \brief Window handling
*/

#ifndef INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_
#define INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_

#include <unordered_set>
#include <memory>

#include <GLFW/glfw3.h>

#include <simple3d/types.h>
#include <simple3d/context/input.h>

namespace Simple3D {



class Window {
 public:
  friend App;

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  ~Window();

 private:
  static Window Create(Error* error);

  Window() = default;
  explicit Window(GLFWwindow* window);
  Window(Window&&);
  Window& operator=(Window&&);

  void LoadGLES2();

  bool ShouldClose();
  void SwapBuffers();
  void EnableInputHandler(std::shared_ptr<IInputHandler> input_handler);
  void EnableWindowInputHandler(
    std::shared_ptr<IWindowInputHandler> window_input_handler);
  void DisableInputHandler(std::shared_ptr<IInputHandler> input_handler);
  void DisableWindowInputHandler(
    std::shared_ptr<IWindowInputHandler> window_input_handler);

  GLFWwindow* window_{nullptr};
  std::unordered_set<std::shared_ptr<IInputHandler>> input_handlers_{};
  std::unordered_set<std::shared_ptr<IWindowInputHandler>> window_input_handlers_{};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_CONTEXT_WINDOW_H_
