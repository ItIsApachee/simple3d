/** \~Russian
 * \file
 * \brief Интерфейс IRenderer.
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_RENDERER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_RENDERER_H_

#include <unordered_map>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <simple3d/types.h>
#include <simple3d/graphics/scene.h>
#include <simple3d/graphics/shader.h>

namespace Simple3D {



/** \~Russian
 * \class IRenderer
 * \brief Интерфейс рендерера.
 * 
 * Абстракция над объектом, отвечающим за создание
 * и отрисовку объектов на сцене. Типичная реализация
 * пользовательского реднерера должна унаследоваться
 * от данного интерфейса, реализовать методы, а также
 * предоставить способ для создания объектов в виде
 * методов M* Create(T1 arg1, T2, arg2, ...);, которые
 * возвращают указатель на объект, предоставляющий
 * пользователю функционал для управления объектом:
 * цвет, позиция, поворот объекта и прочее.
 * Также реализация рендерера R должна содержать публичный
 * тип или псевдоним типа R\::Shader, который должен быть
 * унаследован от Simple3D::IShader. Экземпляр данного типа
 * будет передан Simple3D::View в вызове IRenderer::Draw.
 * 
 * Подробнее про R\::Shader в описании Simple3D::IShader.
 * Подробнее про использование IRenderer в описании
 * Simple3D::Scene::Create.
*/
class IRenderer {
 public:
  IRenderer() = default;
  virtual ~IRenderer() = default;
  
  /** \~Russian
   * \brief Метод для отрисовки объектов рендерера.
   * \param[in] shader Указатель на шейдер, который
   * может быть безопасно динамически преобразован
   * к R\::Shader, где R - некая реализация интерфейса.
   *
   * Вызов данного метода должен отрисовать объекты,
   * хранимые рендерером, используя данный шейдер.
  */
  virtual void Draw(IShader* shader) = 0;

  /** \~Russian
   * \brief Метод для уведомления рендерера о том,
   * что объект по ардесу ptr был изменен.
   * 
   * Используется для оптимизации рендерером. Предоставляет возможность
   * реже обновлять данные, необходимые для отрисовки, давая рендереру
   * понять, изменился ли объект при следующем вызове IRenderer::Draw.
  */
  virtual void NotifyUpdated(void* ptr) = 0;

  /** \~Russian
   * \brief Метод для уведомления рендерера о том,
   * что объект по адресу ptr больше не используется.
   * 
   * Данный метод предоставляет рендереру способ понять, когда
   * объект больше не используется, позволяя больше его не отрисовывать,
   * освободить память, используемую для хранения данного объекта, и прочее.
  */
  virtual void Destroy(void* ptr) = 0;
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_RENDERER_H_
