/** \~Russian
 * \file
 * \brief Интерфейс Simple3D::IShader.
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_

#include <simple3d/graphics/light.h>
#include <simple3d/types.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <unordered_set>

namespace Simple3D {

/** \~Russian
 * \brief Минимальное количество источников направленного
 * освещения, которое должна поддерживать любой шейдер,
 * используемый для отрисовки.
*/
constexpr int kMinimumDirectionalLights = 8;

/** \~Russian
 * \class IShader
 * \brief Интерфейс шейдера, используемого для отрисовки.
 * 
 * Абстракция над шейдером OpenGL (или их коллекции).
 * Предоставляет пользователю свободу выбора в написании шейдера.
*/
class IShader {
 public:
  IShader() = default;
  virtual ~IShader() = default;

  /** \~Russian
   * \brief Метод для установки view матрицы MVP модели.
   * \param[in] view Матрица view.
  */
  virtual void SetView(const glm::mat4& view) = 0;

  /** \~Russian
   * \brief Метод для установки projection матрицы MVP модели.
   * \param[in] projection Матрица projection
  */
  virtual void SetProj(const glm::mat4& projection) = 0;

  /** \~Russian
   * \brief Метод для установки позиции наблюдателя pos.
   * \param[in] pos Позиция наблюдателя.
   * 
   * Позиция наблюдателя нужна для расчета освещения глянцевых бликов
   * по модели затенения по фонгу.
  */
  virtual void SetViewPos(const glm::vec3& pos) = 0;

  /** \~Russian
   * \brief Метод для установки направленных источников света.
   * \param[in] lights Направленные источники света.
  */
  virtual void SetDirectionalLights(
      const std::unordered_set<std::shared_ptr<DirectionalLight>>& lights) = 0;

  /** \~Russian
   * \brief Метод для установки фонового освещения.
   * \param[in] color Цвет фонового освещения.
  */
  virtual void SetAmbientLight(const glm::vec3& color) = 0;

  /** \~Russian
   * \brief Метод для активации шейдера.
   * 
   * Перед вызовом R\::Draw для какого-то рендерера R
   * вызывается S\::Use() для некоторого экземпляра
   * типа S, который является шейдером ассоциированным с R.
  */
  virtual void Use() const = 0;
};

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_SHADER_H_
