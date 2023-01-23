/** \~Russian
 * \file
 * \brief Обработка ввода.
*/
#ifndef INCLUDE_SIMPLE3D_CONTEXT_INPUT_H_
#define INCLUDE_SIMPLE3D_CONTEXT_INPUT_H_

#include <GLFW/glfw3.h>

namespace Simple3D {

/** \~Russian
 * \class IInputHandler
 * \brief Интерфейс для обработки ввода.
 * 
 * По умолчанию методы ничего не делают. Для реализации своей обработки ввода
 * нужно создать класс, наследующийся от IInputHandler со своей реализацией
 * обработки нужных событий.
 * Методы представляют собой обертку над callback функциями GLFW. Названия
 * callback функций такое же как и в GLFW. Подробнее
 * об аргументах, с которыми они вызываются можно узнать по ссылке
 * https://www.glfw.org/docs/3.3/group__input.html
*/
class IInputHandler {
 public:
  IInputHandler() = default;
  virtual ~IInputHandler() = default;

  virtual void KeyCallback(int key, int scancode, int action, int mods);
  virtual void CharCallback(unsigned int codepoint);
  virtual void CharModsCallback(unsigned int codepoint, int mods);
  virtual void MouseButtonCallback(int button, int action, int mods);
  virtual void CursorPosCallback(double xpos, double ypos);
  virtual void CursorEnterCallback(int entered);
  virtual void ScrollCallback(double xoffset, double yoffset);
  virtual void DropCallback(int path_count, const char* paths[]);
};

/** \~Russian
 * \class IWindowInputHandler
 * \brief Интерфейс для обработки событий окна.
 * 
 * По умолчанию методы ничего не делают. Для реализации своей обработки
 * событий окна нужно создать класс, наследующийся от IWindowInputHandler
 * со своей реализацией обработки нужных событий.
 * Методы представляют собой обертку над callback функциями GLFW. Названия
 * callback функций такое же как и в GLFW. Подробнее
 * об аргументах, с которыми они вызываются можно узнать по ссылке
 * https://www.glfw.org/docs/latest/group__window.html
*/
class IWindowInputHandler {
 public:
  IWindowInputHandler() = default;
  virtual ~IWindowInputHandler() = default;

  virtual void WindowPosCallback(int xpos, int ypos);
  virtual void WindowSizeCallback(int width, int height);
  virtual void WindowCloseCallback();
  virtual void WindowRefreshCallback();
  virtual void WindowFocusCallback(int focused);
  virtual void WindowIconifyCallback(int iconified);
  virtual void WindowMaximizeCallback(int maximized);
  virtual void FramebufferSizeCallback(int width, int height);
  virtual void WindowContentScaleCallback(float xscale, float yscale);
};

namespace Internal {

/** \~Russian
 * \brief Функция для установки обработчика ввода для окна GLFW.
 * \warning Это часть внутренней структуры библиотеки. Пользователь не должен
 * сам вызывать эту функцию. Она будет вызвана при инициализации приложения.
*/
void SetInputHandler(GLFWwindow* window, IInputHandler*);

/** \~Russian
 * \brief Функция для отключения обработки ввода для окна GLFW.
 * \warning Это часть внутренней структуры библиотеки. Пользователь не должен
 * сам вызывать эту функцию. Она будет вызвана при инициализации приложения.
*/
void UnsetInputHandler();

/** \~Russian
 * \brief Функция для установки обработчика событий окна GLFW.
 * \warning Это часть внутренней структуры библиотеки. Пользователь не должен
 * сам вызывать эту функцию. Она будет вызвана при завершении работы приложения.
*/
void SetWindowInputHandler(GLFWwindow* window, IWindowInputHandler*);

/** \~Russian
 * \brief Функция для отключения обработки событий окна GLFW.
 * \warning Это часть внутренней структуры библиотеки. Пользователь не должен
 * сам вызывать эту функцию. Она будет вызвана при завершении работы приложения.
*/
void UnsetWindowInputHandler();

}  // namespace Internal

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_CONTEXT_INPUT_H_
