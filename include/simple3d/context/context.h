/** \~Russian
 * \dir
 * \brief Управление ресурсами.
 * 
 * В данной директории представлены объекты, отвечающие за инициализацию
 * приложения, инициализация графических контекстов и обработку ввода.
 */
/** \~Russian
 * \file
 * \brief Объект App.
 */

#ifndef INCLUDE_SIMPLE3D_CONTEXT_CONTEXT_H_
#define INCLUDE_SIMPLE3D_CONTEXT_CONTEXT_H_

#include <GLFW/glfw3.h>
#include <glad/gles2.h>
#include <simple3d/context/window.h>
#include <simple3d/misc/error.h>
#include <simple3d/types.h>

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_set>

namespace Simple3D {

/** \~Russian
 * \class AppConfig
 * \brief Конфигурация приложения.
*/
struct AppConfig {
 public:
  /** \~Russian
   * \property window_title
   * \brief Заголовок окна.
  */
  std::string window_title{"Application"};

  /** \~Russian
   * \property window_maximized
   * \brief Логический флаг, отвечающий за то, будет ли созданное окно
   * максимизированным.
  */
  bool window_maximized{false};
};

/** \~Russian
 * \class App 
 * \brief Класс синглтон, управляющий окном и графическим контекстом.
 */
class App {
 public:
  /** \~Russian
   * \brief Метод для получения экземпляра синглтона.
  */
  static App& GetInstance();

  /** \~Russian
   * \brief Метод для инициализации приложения.
   * \param[in] cfg Конфиг для инициализации приложения.
   * \return Возвращает Error::Ok() при удачной инициализации, иначе ошибку.
  */
  static Error Init(const AppConfig& cfg = {});

  /** \~Russian
   * \brief Метод для завершения работы приложения.
  */
  static void Destroy();

  /** \~Russian
   * \brief Проверяет, нужно ли завершить работу приложения.
   * \return Логический флаг. Если приложению стоит завершить работу, то он
   * равен true, иначе false.
  */
  static bool ShouldClose();

  /** \~Russian
   * \brief Метод для смены буферов отрисовки.
   * 
   * Вызов данного метода меняет местами отображаемый на экране буфер с
   * буфером, на который происходит отрисовка. Данный метод нужно вызывать
   * после отрисовки каждого кадра.
  */
  static void SwapBuffers();

  /** \~Russian
   * \brief Метод для поллинга событий от системы.
   * 
   * Данный метод вызывает callback-функции, которые обрабатывают события от
   * системы: передвижение окна, движение мыши, ввод с клавиатуры и прочее.
  */
  static void PollEvents();

  /** \~Russian
   * \brief Метод для добавление нового обработчика ввода.
   * \param[in] input_handler Новый обработчик ввода.
  */
  static void EnableInputHandler(std::shared_ptr<IInputHandler> input_handler);

  /** \~Russian
   * \brief Метод для добавления нового обработчика событий окна.
   * \param[in] window_input_handler Новый обработчик событий окна.
  */
  static void EnableWindowInputHandler(
      std::shared_ptr<IWindowInputHandler> window_input_handler);

  /** \~Russian
   * \brief Метод для отключения обработчика ввода.
   * \param[in] input_handler Отключаемый обработчик ввода.
  */
  static void DisableInputHandler(std::shared_ptr<IInputHandler> input_handler);

  /** \~Russian
   * \brief Метод для отключения обработчика событий окна.
   * \param[in] window_input_handler Отключаемый обработчик событий окна.
  */
  static void DisableWindowInputHandler(
      std::shared_ptr<IWindowInputHandler> window_input_handler);

  /** \~Russian
   * \brief Метод для получения окна GLFW.
   * \return Окно GLFW.
  */
  static GLFWwindow* App::GetGLFWwindow();

  /** \~Russian
   * \brief Метод для получения уникального идентификатора контекста.
   * \return Уникальный идентификатор контекста. Значение 0 значит, что
   * приложение не инициализировано.
   * 
   * Уникальность гарантируется только в контексте конкретного запуска
   * приложения. Уникальный идентификатор контекста используется для связывания
   * объектов OpenGL ES с контекстом, в котором они были созданы.
  */
  static std::int64_t GetCtxId();

  App(const App&) = delete;
  App(App&&) = delete;
  App& operator=(const App&) = delete;
  App& operator=(App&&) = delete;

  ~App() = default;

 private:
  App() = default;

  bool test() {}

  // bool is_init{false};
  std::int64_t ctx_id_{0};
  std::int64_t last_ctx_id_{0};

  Window window_{};
};

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_CONTEXT_CONTEXT_H_
