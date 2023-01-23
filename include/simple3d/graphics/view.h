/** \~Russian
 * \file
 * \brief Класс Simple3D::View.
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_VIEW_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_VIEW_H_

#include <simple3d/types.h>
#include <simple3d/misc/error.h>
#include <simple3d/graphics/scene.h>

namespace Simple3D {



/** \~Russian
 * \class View
 * \brief Объект, который может отрисовать сцену.
 * 
 * \note Не является синглтоном, для того, чтобы
 * в будущем добавить возможность рисовать не только в окно.
*/
class View {
 public:
  View();  // create framebuffer
  View(const View&) = delete;
  View(View&&) = default;
  View& operator=(const View&) = delete;
  View& operator=(View&&) = default;
  ~View();  // delete framebuffer if not 0

  /** \~Russian
   * \brief Метод для отрисовки сцены.
   * \param[in] scene Отрисовываемая сцена.
   * \return Ошибка в случае, если шейдер, требуемый для отрисовки не
   * инициализирован. Иначе Error::Ok{};
  */
  Error Draw(const Scene& scene);

 private:
  // unsigned int framebuffer_{0};
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_VIEW_H_
