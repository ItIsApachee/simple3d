/** \~Russian
 * \file
 * \brief Освещение.
 * 
 * Данная библиотека использует модифицированное затенение по фонгу, как
 * модель расчета освещения. Все освещение делится на 3 типа:
 * - Фоновое (ambient)
 * - Рассеянное (diffuse)
 * - Глянцевые блики (specular)
 * Фоновое и рассеяное освещение для расчетов используют
 * diffuse цвет. Глянцевые блики - specular цвет.
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_LIGHT_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_LIGHT_H_

#include <glad/gles2.h>

#include <glm/vec3.hpp>

namespace Simple3D {



/** \~Russian
 * \struct DirectionalLight
 * \brief Направленный свет.
*/
struct DirectionalLight {
  /** \~Russian
   * \brief Направление света.
  */
  glm::vec3 direction = glm::vec3(0.0f);

  /** \~Russian
   * \brief diffuse цвет для фонового и рассеяного освещения.
  */
  glm::vec3 diffuse = glm::vec3(0.0f);

  /** \~Russian
   * \brief specular цвет для освещения глянцевых бликов.
  */
  glm::vec3 specular = glm::vec3(0.0f);
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_LIGHT_H_
