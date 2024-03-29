/** \~Russian
 * \file
 * \brief Класс Simple3D::PrimitiveRenderer<P>.
*/
#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_PRIMITIVE_RENDERER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_PRIMITIVE_RENDERER_H_

#include <glad/gles2.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <simple3d/graphics/internal/element_buffer_object.h>
#include <simple3d/graphics/internal/vertex.h>
#include <simple3d/graphics/internal/vertex_array_object.h>
#include <simple3d/graphics/internal/vertex_buffer_object.h>
#include <simple3d/graphics/models/model_shader.h>
#include <simple3d/graphics/renderer.h>

#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Simple3D {

namespace Internal {

/** \~Russian
 * \struct PrimitiveInstance
 * \brief Структура экземпляра примитива, используемая для рендера.
 * 
 * Данная структура предназначена для унификации параметров примитива.
*/
struct PrimitiveInstance {
  /** \~Russian
   * \brief Привязка активного Internal::VertexBufferObject к Internal::VertexArrayObject.
   * 
   * Используется для связаываня данных буфера вершин к массиву вершин.
   * Буфер вершин должен быть массивом Internal::PrimitiveInstance.
   * Работает только при использовании Simple3D::ModelShader.
  */
  static void BindAttributes();

  /** \~Russian
   * \brief Матрица model модели MVP.
   * 
   * \warning На данный момент Simple3D::PrimitiveRenderer<P> не поддерживает
   * model матрицы, использующие неоднородные преобразования масштабирования.
  */
  glm::mat4 model;

  /** \~Russian
   * \brief Цвет для расчета рассеянного освещения затенения по Фонгу.
  */
  glm::vec3 diffuse_color;

  /** \~Russian
   * \brief Цвет для расчета освещения глянцевых бликов затенения по Фонгу.
  */
  glm::vec3 specular_color;

  /** \~Russian
   * \brief Блеск для расчета освещения глянцевых бликов затенения по Фонгу.
  */
  float shininess;
};

}  // namespace Internal

/*
P is some type of a primitive
P requires that there are static methods
  T1 P::GetVertices();
  T2 P::GetIndices();
where T1 and T2 can be used to initialize
  const std::vector<Internal::Vertex>& and
  const std::vector<GLuint>&
respectively, and
  P::GetVertices returns vertices for rendering,
  P::GetIndices returns indices for rendering

also
explicit operator Internal::PrimitiveInstance() const;
*/

/*
*/
/** \~Russian
 * \class PrimitiveRenderer
 * \brief Рендерер примитивов типа P.
 * \tparam P Тип отрисовываемого примитива. Требования:
 * - Должны быть определены статические методы
 * T1 P\::GetVertices() и T2 P\::GetIndices(),
 * где T1 и T2 могут быть использованы для иницализации
 * const std\::vector<Internal\::Vertex>& и
 * const std\::vector<GLuint>&
 * соответственно, причем
 *   -  P\::GetVertices возвращает вектор вершин,
 *   -  P\::GetIndices возвращает вектор индексов размера \f$ 3 \cdot N \f$, такой, что
 * \f$ \forall i=0,1,...,N-1: \f$ элементы с индексами
 * \f$ (3\cdot i, 3\cdot i + 1, 3\cdot i + 2) \f$
 * определяют индексы вершин очередного треугольника в векторе вершин.
 * - Должен быть определен оператор:
 * explicit P\::operator Internal::PrimitiveInstance() const;
 * 
 * Если для типа P выполняются требования, то на сцене можно создать
 * объект типа P с помощью
 * Scene::Create<P, PrimitiveRenderer<P>>(arg1, arg2,..), а
 * так же можно добавить в тип P публичный псевдоним типа P\::Renderer =
 * PrimitiveRenderer<P>. Во втором случае для создания на сцене
 * объекта типа P можно вызвать
 * Scene::Create<P>(arg1, arg2,...).
*/
template <typename P>
class PrimitiveRenderer : public IRenderer {
 public:
  using Shader = ModelShader;
  /** \~Russian
   * \brief Создает рендерер примитива P.
   * 
   * Создание PrimitiveRenderer при неинициализированном контексте - UB.
  */
  PrimitiveRenderer();
  PrimitiveRenderer(const PrimitiveRenderer&) = delete;
  PrimitiveRenderer(PrimitiveRenderer&&) = default;
  PrimitiveRenderer& operator=(const PrimitiveRenderer&) = delete;
  PrimitiveRenderer& operator=(PrimitiveRenderer&&) = default;
  ~PrimitiveRenderer() override = default;

  void Draw(IShader*) override;

  void NotifyUpdated(void*) override;
  void Destroy(void*) override;

  /** \~Russian
   * \brief Метод для создания примитива.
   * \tparam Args Типы аргументов, передаваемых конструктору примитива.
   * \param[in] args Аргументы, передаваемые конструктору примитива.
   * \return Указатель на созданный примитив.
   * 
   * Данная функция использует прямую передачу, для
   * конструирования объекта типа P, используя
   * конструктор данного типа.
   * Данная функция используется при вызове
   * Scene::Create<P, PrimitiveRenderer<P>>(arg1, arg2, ...).
  */
  template <typename... Args>
  P* Create(Args&&... args);

 private:
  static constexpr GLenum kInstanceVboUsage = GL_DYNAMIC_DRAW;

  bool reinitialize_instances{false};
  std::vector<P*> updated_primitives{};

  std::unordered_map<P*, std::pair<std::unique_ptr<P>, std::int64_t>>
      primitives_{};

  Internal::ElementBufferObject ebo_{};
  Internal::VertexBufferObject verices_vbo_{};
  GLsizei indices_count_{};

  Internal::VertexBufferObject instances_vbo_{};
  std::size_t instances_vbo_capacity_{0};

  std::vector<Internal::PrimitiveInstance> instances_{};

  Internal::VertexArrayObject vao_{};
};

// implementation
template <typename P>
template <typename... Args>
P* PrimitiveRenderer<P>::Create(Args&&... args) {
  auto primitive_ptr = std::unique_ptr<P>(new P(std::forward<Args>(args)...));
  auto primitive_ptr_val = primitive_ptr.get();
  primitives_.emplace(primitive_ptr_val,
                      std::pair{std::move(primitive_ptr), -1});
  updated_primitives.push_back(primitive_ptr_val);

  return primitive_ptr_val;
}

template <typename P>
PrimitiveRenderer<P>::PrimitiveRenderer()
    : vao_(),
      verices_vbo_(),
      instances_vbo_(),
      instances_vbo_capacity_{0},
      ebo_(),
      primitives_() {
  vao_.Bind();

  // FIXME: define overloadable function that should be used
  // to get vertices & indices
  const std::vector<Internal::Vertex>& primitive_vertices = P::GetVertices();
  const std::vector<GLuint>& primitive_indices = P::GetIndices();
  indices_count_ = primitive_indices.size();

  verices_vbo_ = Internal::VertexBufferObject(
      sizeof(Internal::Vertex) * primitive_vertices.size(),
      (const std::byte*)(primitive_vertices.data()));
  verices_vbo_.Bind();
  Internal::Vertex::BindAttributes();

  instances_vbo_ = Internal::VertexBufferObject(0, nullptr, kInstanceVboUsage);
  instances_vbo_.Bind();
  Internal::PrimitiveInstance::BindAttributes();

  ebo_ = Internal::ElementBufferObjectBuilder()
             .Data(sizeof(GLuint) * primitive_indices.size(),
                   (const std::byte*)(primitive_indices.data()))
             .Build(vao_);
  vao_.BindEbo(ebo_);
  vao_.Unbind();
}

template <typename P>
void PrimitiveRenderer<P>::Draw(IShader* shader_) {
  if (primitives_.empty()) {
    return;
  }

  // resize VBO buffer
  constexpr std::int64_t kChunkSize = 64;  // cubes are stored in chunks of 64

  // if true, need to reload everything in instances_vbo
  bool flag_full_sub_data = reinitialize_instances;

  if (primitives_.size() > instances_vbo_capacity_) {
    flag_full_sub_data = true;

    instances_vbo_capacity_ =
        (primitives_.size() + kChunkSize - 1) / kChunkSize * kChunkSize;
    instances_vbo_.SetData(
        instances_vbo_capacity_ * sizeof(Internal::PrimitiveInstance), nullptr,
        kInstanceVboUsage);
  }
  if (instances_vbo_capacity_ - (std::int64_t)primitives_.size() > kChunkSize) {
    flag_full_sub_data = true;

    instances_vbo_capacity_ =
        (primitives_.size() + kChunkSize - 1) / kChunkSize * kChunkSize;
    instances_vbo_.SetData(
        instances_vbo_capacity_ * sizeof(Internal::PrimitiveInstance), nullptr,
        kInstanceVboUsage);
  }

  std::size_t instances_cnt = primitives_.size();

  // important: should be instances_vbo_capacity_
  // otherwise segfault (pointer out of bounds)
  instances_.reserve(instances_vbo_capacity_);

  std::int64_t prev_size = instances_.size();
  if (reinitialize_instances) {
    instances_.clear();

    for (auto& [primitive_ptr, val] : primitives_) {
      // FIXME: create overloadable function
      // to get model, and colors
      auto& primitive = *primitive_ptr;
      auto& [primitive_unique_ptr_, index] = val;

      instances_.emplace_back(
          static_cast<Internal::PrimitiveInstance>(primitive));
    }
  } else {
    for (auto& primitive_ptr : updated_primitives) {
      auto& primitive = *primitive_ptr;

      glm::mat4 model(1.0f);
      glm::vec3 pos(primitive.pos);
      model = glm::translate(model, pos);
      if (auto& index = primitives_[primitive_ptr].second; index == -1) {
        index = instances_.size();
        instances_.emplace_back(
            static_cast<Internal::PrimitiveInstance>(primitive));
      } else {
        instances_[index] = static_cast<Internal::PrimitiveInstance>(primitive);
      }
    }
  }
  updated_primitives.clear();

  // TODO(apachee): optimize memory copies
  instances_vbo_.SubData(0, instances_cnt * sizeof(Internal::PrimitiveInstance),
                         (const std::byte*)instances_.data());

  vao_.Bind();
  glDrawElementsInstanced(GL_TRIANGLES, indices_count_, GL_UNSIGNED_INT,
                          reinterpret_cast<void*>(0),
                          static_cast<GLsizei>(instances_cnt));
  vao_.Unbind();
}

template <typename P>
void PrimitiveRenderer<P>::NotifyUpdated(void* primitive_void) {
  P* primitive_ptr = reinterpret_cast<P*>(primitive_void);
  updated_primitives.push_back(primitive_ptr);
}

template <typename P>
void PrimitiveRenderer<P>::Destroy(void* primitive_void) {
  P* primitive_ptr = reinterpret_cast<P*>(primitive_void);
  primitives_.erase(primitive_ptr);
  reinitialize_instances = true;
}

}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_MODELS_PRIMITIVE_RENDERER_H_
