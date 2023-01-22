#ifndef INCLUDE_SIMPLE3D_GRAPHICS_MODELS_PRIMITIVE_RENDERER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_MODELS_PRIMITIVE_RENDERER_H_

#include <glad/gles2.h>
#include <simple3d/graphics/internal/element_buffer_object.h>
#include <simple3d/graphics/internal/vertex.h>
#include <simple3d/graphics/internal/vertex_array_object.h>
#include <simple3d/graphics/internal/vertex_buffer_object.h>
#include <simple3d/graphics/models/model_shader.h>
#include <simple3d/graphics/renderer.h>

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include <type_traits>

namespace Simple3D {

namespace Internal {
struct PrimitiveInstance {
  static void BindAttributes();
  glm::mat4 model;
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
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

template <typename P>
class PrimitiveRenderer : public IRenderer {
 public:
  using Shader = ModelShader;
  PrimitiveRenderer();
  PrimitiveRenderer(const PrimitiveRenderer&) = delete;
  PrimitiveRenderer(PrimitiveRenderer&&) = default;
  PrimitiveRenderer& operator=(const PrimitiveRenderer&) = delete;
  PrimitiveRenderer& operator=(PrimitiveRenderer&&) = default;
  ~PrimitiveRenderer() override = default;

  void Draw(IShader&) override;

  void NotifyUpdated(void*) override;
  void Destroy(void*) override;

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

  std::cerr << "vertices: " << std::endl;
  for (auto vertex : primitive_vertices) {
    std::cerr << vertex.position.x << ' ' << vertex.position.y << ' '
              << vertex.position.z << std::endl;
  }

  std::cerr << "indices: " << std::endl;
  for (auto index : primitive_indices) {
    std::cerr << index << std::endl;
  }
}

template <typename P>
void PrimitiveRenderer<P>::Draw(IShader& shader_) {
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
  glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT,
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
