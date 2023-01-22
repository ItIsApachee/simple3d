#include <simple3d/graphics/models/cuboid.h>

#include <array>
#include <cstddef>

// FIXME: don't include everything
#include <simple3d/graphics/internal/element_buffer_object.h>
#include <simple3d/graphics/internal/vertex.h>
#include <simple3d/graphics/internal/vertex_array_object.h>
#include <simple3d/graphics/internal/vertex_buffer_object.h>
#include <simple3d/graphics/models/model_shader.h>
#include <simple3d/graphics/renderer.h>
#include <simple3d/graphics/scene.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Simple3D {

Cuboid::Cuboid(const glm::vec3& pos, const glm::vec3& diffuse_color,
               const glm::vec3& specular_color, const GLfloat& shininess)
    : pos{pos},
      diffuse_color{diffuse_color},
      specular_color{specular_color},
      shininess{shininess} {}

static constexpr auto GenVertices() {
  // TODO(apachee): fix order for face culling to work

  constexpr int faces_cnt = 6;
  constexpr int vertices_per_face = 4;
  constexpr int vertices_cnt = faces_cnt * vertices_per_face;

  std::array<Internal::Vertex, vertices_cnt> result{};

  std::array<glm::vec3, 3> axis = {glm::vec3{1.f, 0.f, 0.f},
                                   glm::vec3{0.f, 1.f, 0.f},
                                   glm::vec3{0.f, 0.f, 1.f}};

  // 6 {normal, axis} pairs for faces
  // normal is collinear with axis
  std::array<std::pair<glm::vec3, int>, 6> faces = {
      std::pair{axis[0], 0}, {-axis[0], 0}, {axis[1], 1},
      {-axis[1], 1},         {axis[2], 2},  {-axis[2], 2},
  };

  for (int i = 0; i < faces_cnt; i++) {
    auto [normal, axis_id] = faces[i];
    int shift_axis0_id = 0 + axis_id == 0;
    int shift_axis1_id = shift_axis0_id + 1 + (axis_id == shift_axis0_id + 1);
    glm::vec3 shift_axis0 = axis[shift_axis0_id];
    glm::vec3 shift_axis1 = axis[shift_axis1_id];

    result[vertices_per_face * i + 0] =
        Internal::Vertex{normal + shift_axis0 + shift_axis1, normal};
    result[vertices_per_face * i + 1] =
        Internal::Vertex{normal + shift_axis0 - shift_axis1, normal};
    result[vertices_per_face * i + 2] =
        Internal::Vertex{normal - shift_axis0 - shift_axis1, normal};
    result[vertices_per_face * i + 3] =
        Internal::Vertex{normal - shift_axis0 + shift_axis1, normal};
  }

  for (int i = 0; i < vertices_cnt; i++) {
    result[i].position /= 2.f;
  }

  return result;
}

static constexpr auto GenIndices() {
  constexpr int faces_cnt = 6;
  constexpr int vertices_per_face = 4;
  constexpr int indices_per_face = 6;
  constexpr int indices_cnt = faces_cnt * indices_per_face;

  std::array<GLuint, indices_cnt> indices{};
  for (int i = 0; i < faces_cnt; i++) {
    indices[indices_per_face * i + 0] = vertices_per_face * i + 0;
    indices[indices_per_face * i + 1] = vertices_per_face * i + 1;
    indices[indices_per_face * i + 2] = vertices_per_face * i + 2;
    indices[indices_per_face * i + 3] = vertices_per_face * i + 2;
    indices[indices_per_face * i + 4] = vertices_per_face * i + 3;
    indices[indices_per_face * i + 5] = vertices_per_face * i + 0;
  }

  return indices;
}

static constexpr auto kCuboidVertices = GenVertices();
static constexpr auto kCuboidIndices = GenIndices();

const std::vector<Internal::Vertex>& Cuboid::GetVertices() {
  static const std::vector<Internal::Vertex>& vertices =
      *new std::vector<Internal::Vertex>(kCuboidVertices.begin(),
                                         kCuboidVertices.end());
  return vertices;
}

const std::vector<GLuint>& Cuboid::GetIndices() {
  static const std::vector<GLuint>& vertices =
      *new std::vector<GLuint>(kCuboidIndices.begin(), kCuboidIndices.end());
  return vertices;
}

Cuboid::operator Internal::PrimitiveInstance() const {
  glm::mat4 model(1.0f);
  glm::vec3 pos(pos);
  model = glm::translate(model, pos);

  return {model, diffuse_color, specular_color, shininess};
}

}  // namespace Simple3D
