/** \~Russian
 * \file
 * \brief Примитив куб.
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_

#include <glad/gles2.h>
#include <simple3d/graphics/models/primitive_renderer.h>
#include <simple3d/types.h>

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Simple3D {

/** \~Russian
 * \struct Cuboid
 * \brief Примитив куб.
 * 
 * Название Cuboid, чтобы в будущем преобразовать куб
 * к прямоугольному параллелепипеду.
*/
struct Cuboid {
 public:
  // TODO(apachee): add ability to change face's color

  /** \~Russian
   * \brief Рендерер, используемый при вызове Scene::Create<Cuboid>(...);
  */
  using Renderer = PrimitiveRenderer<Cuboid>;

  /** \~Russian
   * \brief Реализация требований на шаблонный параметр P типа PrimitiveRenderer.
  */
  static const std::vector<Internal::Vertex>& GetVertices();

  /** \~Russian
   * \brief Реализация требований на шаблонный параметр P типа PrimitiveRenderer.
  */
  static const std::vector<GLuint>& GetIndices();

  Cuboid(const glm::vec3& pos = glm::vec3(0.0f),
         const glm::vec3& diffuse_color = glm::vec3(1.0f),
         const glm::vec3& specular_color = glm::vec3(1.0f),
         const GLfloat& shininess = 16.0f);
  Cuboid(const Cuboid&) = default;
  Cuboid& operator=(const Cuboid&) = default;
  ~Cuboid() = default;

  /** \~Russian
   * \brief Реализация требований на шаблонный параметр P типа PrimitiveRenderer.
  */
  explicit operator Internal::PrimitiveInstance() const;

  /** \~Russian
   * \brief Позиция куба.
  */
  glm::vec3 pos = glm::vec3(0.0f);


  /** \~Russian
   * \brief Цвет используемый для расчета фонового и рассеянного освещения.
  */
  glm::vec3 diffuse_color = glm::vec3(1.0f);


  /** \~Russian
   * \brief Цвет используемый для расчета освещения глянцевых бликов.
  */
  glm::vec3 specular_color = glm::vec3(1.0f);


  /** \~Russian
   * \brief Блеск используемый для расчета освещения глянцевых бликов.
  */
  GLfloat shininess{16.0f};

  // TODO(apachee): add rotation
  // // rotation
  // float roll{0.};
  // float pitch{0.};
  // float yaw{0.};
};

// struct CuboidInstance;
//
// class CuboidRenderer : public IRenderer {
//  public:
//   using Shader = ModelShader;
//
//   CuboidRenderer();
//   CuboidRenderer(const CuboidRenderer&) = delete;
//   CuboidRenderer(CuboidRenderer&&) = default;
//   CuboidRenderer& operator=(const CuboidRenderer) = delete;
//   CuboidRenderer& operator=(CuboidRenderer&&) = default;
//   ~CuboidRenderer() override = default;
//
//   template <typename... Args>
//   Cuboid* Create(Args&&... args);
//
//   void Draw(IShader&) override;
//
//   void NotifyUpdated(void*) override;
//   void Destroy(void*) override;
//
//  private:
//   bool reinitialize_instances{false};
//   std::vector<Cuboid*> updated_cuboids{};
//
//   std::unordered_map<Cuboid*, std::pair<std::unique_ptr<Cuboid>,
//   std::int64_t>>
//       cuboids_{};
//
//   Internal::ElementBufferObject ebo_{};
//   Internal::VertexBufferObject verices_vbo_{};
//
//   Internal::VertexBufferObject instances_vbo_{};
//   std::size_t instances_vbo_capacity_{0};
//
//   std::vector<CuboidInstance> instances_{};
//
//   Internal::VertexArrayObject vao_{};
// };
//
//// implementation
// template <typename... Args>
// Cuboid* CuboidRenderer::Create(Args&&... args) {
//   // Cuboid* cuboid_ptr =  new Cuboid{std::forward<Args>(args)...};
//   auto cuboid_ptr =
//       std::unique_ptr<Cuboid>(new Cuboid{std::forward<Args>(args)...});
//   auto cuboid_ptr_val = cuboid_ptr.get();
//   cuboids_.emplace(cuboid_ptr_val, std::pair{std::move(cuboid_ptr), -1});
//   updated_cuboids.push_back(cuboid_ptr_val);
//
//   return cuboid_ptr_val;
// }

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_CUBOID_H_
