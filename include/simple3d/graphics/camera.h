/** \~Russian
 * \file
 * \brief Камера.
 */

#ifndef INCLUDE_SIMPLE3D_GRAPHICS_CAMERA_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_CAMERA_H_

#include <glad/gles2.h>
#include <simple3d/types.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace Simple3D {

/** \~Russian
 * \class ICamera
 * \brief Интерфейс камеры.
 * 
 * Интерфейс используются для описания сущности, которая используется для
 * наблюдения сцены. Данный интерфейс отвечает за предоставление view и
 * projection матриц модели MVP (model-view-projection), а также позиции
 * наблюдателя.
*/
class ICamera {
 public:
  ICamera() = default;
  virtual ~ICamera() = default;

  /** \~Russian
   * \brief Метод для получения view матрицы MVP модели.
   * \return Матрица view.
  */
  virtual glm::mat4 GetView();

  /** \~Russian
   * \brief Метод для получения projection матрицы MVP модели.
   * \return Матрица projectioon
  */
  virtual glm::mat4 GetProj();

  /** \~Russian
   * \brief Метод для получения позиции наблюдателя.
   * \return Радиус-вектор позиции наблюдателя.
  */
  virtual glm::vec3 GetViewPos();
};

/** \~Russian
 * \class Camera
 * \brief Реализация интерфейса камеры с полями для конфигурации наблюдателя.
 */
class Camera : public ICamera {
 public:
  Camera() = default;
  ~Camera() override = default;

  /** \~Russian
   * \brief Радиус-вектор позиции наблюдателя.
  */
  glm::vec3 pos{0.0f};

  /** \~Russian
   * \brief Крен камеры.
   * 
   * Подробнее: https://clck.ru/32fMLv
  */
  GLfloat roll{0.0f};
  
  /** \~Russian
   * \brief Тангаж камеры.
   * 
   * Подробнее: https://clck.ru/32fMLv
  */
  GLfloat pitch{0.0f};

  /** \~Russian
   * \brief Рыскание камеры.
   * 
   * Подробнее: https://clck.ru/32fMLv
  */
  GLfloat yaw{0.0f};

  /** \~Russian
   * \brief Угол обзора наблюдателя (field of view).
  */
  GLfloat fov{45.0f};

  /** \~Russian
   * \brief Соотношение ширины к высоте.
  */
  GLfloat aspect_ratio{16.0f / 9.0f};

  glm::mat4 GetView() override;
  glm::mat4 GetProj() override;
  glm::vec3 GetViewPos() override;
};

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_CAMERA_H_
