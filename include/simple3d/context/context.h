/**
 * \dir
 * \brief Managing resources
 */
/** \file
 * \brief Context object
 */

#ifndef INCLUDE_SIMPLE3D_CONTEXT_CONTEXT_H_
#define INCLUDE_SIMPLE3D_CONTEXT_CONTEXT_H_

#include <GLFW/glfw3.h>
#include <glad/gles2.h>
#include <simple3d/context/window.h>
#include <simple3d/misc/error.h>
#include <simple3d/types.h>

#include <memory>
#include <string>
#include <unordered_set>

namespace Simple3D {

// TODO(apachee): implement mechanism to close the window
/** \class Context simple3d/context/context.h
 * \brief Class representing application
 */
class App {
 public:
  static App& GetInstance();

  // load everything, create window
  static Error Init();
  // destroy window, unload everything
  static void Destroy();

  static bool ShouldClose();
  static void SwapBuffers();
  static void PollEvents();
  static void EnableInputHandler(std::shared_ptr<IInputHandler> input_handler);
  static void EnableWindowInputHandler(
      std::shared_ptr<IWindowInputHandler> window_input_handler);
  static void DisableInputHandler(std::shared_ptr<IInputHandler> input_handler);
  static void DisableWindowInputHandler(
      std::shared_ptr<IWindowInputHandler> window_input_handler);

  static GLFWwindow* App::GetGLFWwindow();

  App(const App&) = delete;
  App(App&&) = delete;
  App& operator=(const App&) = delete;
  App& operator=(App&&) = delete;

  ~App() = default;

 private:
  App() = default;

  bool is_init{false};
  Window window_{};
};

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_CONTEXT_CONTEXT_H_
