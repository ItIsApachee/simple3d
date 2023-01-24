/** \~Russian
 * \dir
 * \brief Компонента для интеграции Dear ImGui.
*/
/** \~Russian
 * \dir
 * \brief Инструменты для интеграции Dear ImGui.
*/
#ifndef INCLUDE_SIMPLE3D_IMGUI_IMGUI_H_
#define INCLUDE_SIMPLE3D_IMGUI_IMGUI_H_

#include <simple3d/context/input.h>

#include <memory>
#include <unordered_set>

/** \~Russian
 * \namespace Simple3D::ImGui
 * \brief Компонента для взаимодействия с Dear ImGui
*/
namespace Simple3D::ImGui {

/** \~Russian
 * \brief Создание контекста Dear ImGui.
 * 
 * Данная функия должна быть вызвана после Simple3D::App::Init() и
 * перед использованием ImGui.
 * Делает то же самое, что Context::GetInstance().CreateContext();
*/
void CreateContext();

/** \~Russian
 * \brief Создание нового кадра ImGui.
 * 
 * Данная функция должна быть вызвана перед началом использования ImGui в
 * каждом кадре.
 * Делает то же самое, что Context::GetInstance().NewFrame();
*/
void NewFrame();

/** \~Russian
 * \brief Отрисовка ImGui.
 * 
 * Данная функция должна быть вызвана в конце кадра после использования функций
 * ImGui.
 * Делает то же самое, что Context::GetInstance().Renderer();
*/
void Render();


/** \~Russian
 * \brief Удаление контекста Dear ImGui.
 * 
 * Данная функция должна быть вызвана перед Simple3D::App::Destroy().
 * Делает то же самое, что Context::GetInstance().DestroyContext();
*/
void DestroyContext();

/** \~Russian
 * \class Context
 * \brief Обертка над контекстом ImGui (синглтон).
*/
class Context {
 public:
  /** \~Russian
   * \brief Метод для получения экземпляра синглтона.
   * \return Экземпляр синглтона.
  */
  static Context& GetInstance();

  Context(const Context&) = delete;
  Context(Context&&) = delete;
  Context& operator=(const Context&) = delete;
  Context& operator=(Context&&) = delete;
  ~Context() = default;

  /** \~Russian
   * \brief Создание контекста Dear ImGui.
   * 
   * Данная функия должна быть вызвана после Simple3D::App::Init() и
   * перед использованием ImGui.
  */
  void CreateContext();

  /** \~Russian
   * \brief Создание нового кадра ImGui.
   * 
   * Данная функция должна быть вызвана перед началом использования ImGui в
   * каждом кадре.
  */
  void NewFrame();

  /** \~Russian
   * \brief Отрисовка ImGui.
   * 
   * Данная функция должна быть вызвана в конце кадра после использования функций
   * ImGui.
  */
  void Render();


  /** \~Russian
   * \brief Удаление контекста Dear ImGui.
   * 
   * Данная функция должна быть вызвана перед Simple3D::App::Destroy().
  */
  void DestroyContext();

 private:
  Context() = default;
};

/** \~Russian
 * \class InputHandler
 * \brief Обработчик ввода и событий окна для ImGui.
 * 
 * Предназначен для делегирования событий ImGui.
 * Пример использования:
 * \code{.cpp}
 * auto imgui_handler = std::make_shared<Simple3D::ImGui::InputHandler>();
 * Simple3D::App::EnableInputHandler(imgui_handler);
 * Simple3D::App::EnableWindowInputHandler(imgui_handler);
 * \endcode
 * 
*/
class InputHandler : public IInputHandler, public IWindowInputHandler {
 public:
  InputHandler();
  InputHandler(const InputHandler&) = delete;
  InputHandler(InputHandler&&) = default;
  InputHandler& operator=(const InputHandler&) = delete;
  InputHandler& operator=(InputHandler&&) = default;
  ~InputHandler() override = default;

  void ToggleInputs(bool enable);

  void EnableInputHandler(const std::shared_ptr<IInputHandler>&);
  void EnableWindowInputHandler(const std::shared_ptr<IWindowInputHandler>&);
  void DisableInputHandler(const std::shared_ptr<IInputHandler>&);
  void DisableWindowInputHandler(const std::shared_ptr<IWindowInputHandler>&);

  void KeyCallback(int key, int scancode, int action, int mods) override;
  void CharCallback(unsigned int codepoint) override;
  void CharModsCallback(unsigned int codepoint, int mods) override;
  void MouseButtonCallback(int button, int action, int mods) override;
  void CursorPosCallback(double xpos, double ypos) override;
  void CursorEnterCallback(int entered) override;
  void ScrollCallback(double xoffset, double yoffset) override;
  void DropCallback(int path_count, const char* paths[]) override;

  void WindowPosCallback(int xpos, int ypos) override;
  void WindowSizeCallback(int width, int height) override;
  void WindowCloseCallback() override;
  void WindowRefreshCallback() override;
  void WindowFocusCallback(int focused) override;
  void WindowIconifyCallback(int iconified) override;
  void WindowMaximizeCallback(int maximized) override;
  void FramebufferSizeCallback(int width, int height) override;
  void WindowContentScaleCallback(float xscale, float yscale) override;

 private:
  GLFWwindow* window{nullptr};
  bool enabled{true};

  std::unordered_set<std::shared_ptr<IInputHandler>> input_handlers_{};
  std::unordered_set<std::shared_ptr<IWindowInputHandler>>
      window_input_handlers_{};
};

}  // namespace Simple3D::ImGui

#endif  // INCLUDE_SIMPLE3D_IMGUI_IMGUI_H_
