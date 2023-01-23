/** \~Russian
 * \dir
 * \brief Вспомогательные инструменты для использования библиотеки.
*/
/** \~Russian
 * \file
 * \brief Класс Simple3D::FpsCameraInputHandler.
*/
#ifndef INCLUDE_SIMPLE3D_UTILS_FPS_CAMERA_H_
#define INCLUDE_SIMPLE3D_UTILS_FPS_CAMERA_H_

#include <GLFW/glfw3.h>
#include <simple3d/context/input.h>
#include <simple3d/graphics/camera.h>

#include <chrono>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <optional>

namespace Simple3D {

/** \~Russian
 * \struct FpsCameraConfig
 * \brief Конфигурация для Simple3D::FpsCameraInputHandler.
*/
struct FpsCameraConfig {
 public:
  /** \~Russian
   * \brief Чувствительность мыши.
  */
  float sensitivity{1.0F};

  /** \~Russian
   * \brief Использование "сырого" ввода для положения мыши, если возможно.
   * 
   * Подробнее: https://www.glfw.org/docs/3.3/input_guide.html#raw_mouse_motion
  */
  bool raw_motion_enabled{false};

  /** \~Russian
   * \brief Скорость движения камеры.
  */
  float movement_speed{15.0F};

  /** \~Russian
   * \brief GLFW идентификатор клавиши для движения вперед.
   * 
   * Список клавиш: https://www.glfw.org/docs/3.3/group__keys.html
  */
  int forwards_key{GLFW_KEY_W};

  /** \~Russian
   * \brief GLFW идентификатор клавиши для движения назад.
   * 
   * Список клавиш: https://www.glfw.org/docs/3.3/group__keys.html
  */
  int backwards_key{GLFW_KEY_S};

  /** \~Russian
   * \brief GLFW идентификатор клавиши для движения влево.
   * 
   * Список клавиш: https://www.glfw.org/docs/3.3/group__keys.html
  */
  int left_key{GLFW_KEY_A};

  /** \~Russian
   * \brief GLFW идентификатор клавиши для движения вправо.
   * 
   * Список клавиш: https://www.glfw.org/docs/3.3/group__keys.html
  */
  int right_key{GLFW_KEY_D};

  /** \~Russian
   * \brief GLFW идентификатор клавиши для движения вверх.
   * 
   * Список клавиш: https://www.glfw.org/docs/3.3/group__keys.html
  */
  int up_key{GLFW_KEY_SPACE};

  /** \~Russian
   * \brief GLFW идентификатор клавиши для движения вниз.
   * 
   * Список клавиш: https://www.glfw.org/docs/3.3/group__keys.html
  */
  int down_key{GLFW_KEY_LEFT_SHIFT};

  /** \~Russian
   * \brief Опциональный GLFW идентификатор клавиши для отключения управления.
   * В случае отсутствия управление отключить нельзя.
   * 
   * После отключения управления придется заново вызвать
   * FpsCameraInputHandler::Enable.
  */
  std::optional<int> escape_key{};
};

/** \~Russian
 * \class FpsCameraInputHandler
 * \brief Обработчик ввода и событий окна для управления камерой.
 * 
 * Пример использования:
 * \code{.cpp}
 * auto camera = std::make_shared<Simple3D::Camera>();
 * scene.SetCamera(camera);
 *
 * auto cam_handler =
 *     std::make_shared<Simple3D::FpsCameraInputHandler>();
 * Simple3D::App::EnableInputHandler(cam_handler);
 * Simple3D::App::EnableWindowInputHandler(cam_handler);
 * cam_handler->Enable(camera);
 * \endcode
*/
class FpsCameraInputHandler : public IInputHandler, public IWindowInputHandler {
 public:
  FpsCameraInputHandler();
  /** \~Russian
   * \brief Создание управления камерой по заданной конфигурации.
  */
  explicit FpsCameraInputHandler(const FpsCameraConfig& cfg);
  ~FpsCameraInputHandler() override = default;

  /** \~Russian
   * \brief Метод для включения управления камерой camera.
   * \param[in] camera Управляемая камера.
  */
  void Enable(const std::shared_ptr<Camera>& camera);

  /** \~Russian
   * \brief Отключение управления камерой.
  */
  void Disable();

  /** \~Russian
   * \brief Метод, проверяющий включено ли управление камерой.
   * \return true, если управление включено, иначе false.
  */
  bool IsEnabled();

  /** \~Russian
   * \brief Метод для обновления параметров камеры.
   * \param[in] delta Количество миллисекунд между прошлым и текущим кадром.
   * Данный параметр используется для того, чтобы высчитать расстояние,
   * на которое изменилось положение.
   * 
   * Данный метод нужно вызывать каждый кадр после Simple3D::App::PollEvents(),
   * чтобы обновить параметры камеры.
  */
  bool Update(const std::chrono::milliseconds& delta);

  void CursorPosCallback(double xpos, double ypos) override;

  void WindowFocusCallback(int focused) override;
  void FramebufferSizeCallback(int width, int height) override;

  /** \~Russian
   * \brief Конфигурация управления камеры.
   * 
   * Данное поле можно изменять на ходу, чтобы изменить управление камерой.
  */
  FpsCameraConfig cfg{};

 private:
  struct KeyStates {
    char forwards{0};
    char backwards{0};
    char left{0};
    char right{0};
    char up{0};
    char down{0};
    bool escape{0};
  };

  KeyStates GetKeyStates();

  GLFWwindow* window_{nullptr};

  std::shared_ptr<Camera> camera_{};

  bool focused_{false};
  double prev_xpos_{0.0f};
  double prev_ypos_{0.0f};

  int prev_cursor_mode{GLFW_CURSOR_NORMAL};
};

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_UTILS_FPS_CAMERA_H_
