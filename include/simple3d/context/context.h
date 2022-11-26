/**
 * \dir
 * \brief Managing resources
*/
/** \file
 * \brief Context object
*/

#ifndef INCLUDE_SIMPLE3D_CONTEXT_CONTEXT_H_
#define INCLUDE_SIMPLE3D_CONTEXT_CONTEXT_H_

#include <unordered_set>
#include <string>
#include <memory>

#include <glad/gles2.h>
#include <GLFW/glfw3.h>

#include <simple3d/types.h>
#include <simple3d/misc/error.h>
#include <simple3d/context/window.h>

namespace Simple3D {



// TODO(apachee): implement mechanism to close the window
/** \class Context simple3d/context/context.h
 * \brief Class representing context for managing resources
*/
class Context {
 public:
  static Context& GetInstance();

  Context(const Context&) = delete;
  Context(Context&&) = delete;
  Context& operator=(const Context&) = delete;
  Context& operator=(Context&&) = delete;

  ~Context() = default;

  // load everything, create window
  static Error Init();
  // destroy window, unload everything
  static void Destroy();

  static void PollEvents();
  static std::shared_ptr<Window> GetWindow();

 private:
  Context();

  std::shared_ptr<Window> window_{nullptr};
  bool is_init{false};
  // store Window (but then GetWindow???)
  // shared_ptr<Window>??? so that there is only one window created, but
  // multiple references
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_CONTEXT_CONTEXT_H_
