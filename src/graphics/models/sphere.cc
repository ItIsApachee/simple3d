#include <glad/gles2.h>
#include <simple3d/graphics/models/sphere.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <iterator>
#include <tuple>
#include <utility>

namespace Simple3D::Internal {

namespace SpehereGeometry {

using geometry_t = std::pair<std::vector<glm::vec3>, std::vector<GLuint>>;

geometry_t GenIcosahedron() {
  std::vector<glm::vec3> positions;

  // positions 1, 2, ..., 10 - points between poles
  // 1, 3, 5, ... are closer to top
  // 2, 4, 6, ... are closer to bottom
  positions.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
  for (int i = 0; i < 10; i++) {
    glm::vec3 pos;
    const GLfloat latitude = glm::atan(0.5f);
    pos.y = glm::sin(latitude);
    if (i % 2) {
      pos.y = -pos.y;
    }

    const GLfloat longitude_step = glm::radians(36.0f);
    pos.x = glm::cos(latitude) * glm::sin(longitude_step * i);
    pos.z = glm::cos(latitude) * glm::cos(longitude_step * i);

    // pos *= radius;
    positions.push_back(pos);
  }
  positions.push_back(glm::vec3(0.0f, -1.0f, 0.0f));

  std::vector<GLuint> indices;

  // top triangles
  for (int i = 0; i < 5; i++) {
    int a = 0;
    int b = 1 + i * 2;
    int c = 1 + ((i + 1) % 5) * 2;

    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(c);
  }
  // middle triangles
  for (int i = 0; i < 10; i++) {
    int a = 1 + i;
    int b = 1 + (i + 1) % 10;
    int c = 1 + (i + 2) % 10;

    // to make a-b-c CCW
    if (i % 2) {
      std::swap(b, c);
    }
    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(c);
  }
  // bottom triangles
  for (int i = 0; i < 5; i++) {
    int a = positions.size() - 1;
    int b = 2 + ((i + 1) % 5) * 2;
    int c = 2 + i * 2;

    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(c);
  }

  assert(indices.size() == 20 * 3);

  return {std::move(positions), std::move(indices)};
}

geometry_t Gen(std::uint64_t detalization) {
  if (detalization == 0) return GenIcosahedron();

  auto [positions, indices_prev] = Gen(detalization - 1);
  // subdivide each triangle
  std::vector<GLuint> indices;
  for (int i = 0; i < indices_prev.size(); i += 3) {
    /*
    Triangle structure:
          a
         / \
        c'- b'
       / \ / \
      b - a'- c
    */
    auto a = indices_prev[i + 0];
    auto b = indices_prev[i + 1];
    auto c = indices_prev[i + 2];

    GLuint ap = positions.size();
    positions.push_back(glm::normalize(positions[b] + positions[c]));

    GLuint bp = positions.size();
    positions.push_back(glm::normalize(positions[a] + positions[c]));

    GLuint cp = positions.size();
    positions.push_back(glm::normalize(positions[a] + positions[b]));

    // hardcoded triangles
    std::vector<GLuint> triangles = {
        a, cp, bp, cp, b, ap, bp, ap, c, cp, ap, bp,
    };
    std::copy(triangles.begin(), triangles.end(), std::back_inserter(indices));
  }
  return {std::move(positions), std::move(indices)};
}

}  // namespace SpehereGeometry

std::pair<std::vector<Internal::Vertex>, std::vector<GLuint>> GenSphereGeometry(
    std::uint64_t detalization, bool normal_lerp) {
  // generate geometry
  auto [positions, pos_indices] = SpehereGeometry::Gen(detalization);

  // generate normals (using CCW propery)
  std::vector<Internal::Vertex> vertices;
  std::vector<GLuint> indices;

  if (normal_lerp) {
    // normals interpolation:
    indices = std::move(pos_indices);
    vertices.reserve(positions.size());
    for (const auto& pos : positions) {
      vertices.push_back(Internal::Vertex{pos, pos});
    }
  } else {
    // no normals interpolation:
    vertices.reserve(indices.size());
    for (std::int64_t i = 0; i < pos_indices.size(); i += 3) {
      glm::vec3 ab = positions[pos_indices[i + 1]] - positions[pos_indices[i]];
      glm::vec3 bc =
          positions[pos_indices[i + 2]] - positions[pos_indices[i + 1]];
      glm::vec3 normal = glm::cross(ab, bc);

      for (int j = 0; j < 3; j++) {
        indices.push_back(vertices.size());
        vertices.push_back(
            Internal::Vertex{positions[pos_indices[i + j]], normal});
      }
    }
  }


  constexpr GLfloat radius = 0.5f;  // so that diameter is 1
  for (auto& vertex : vertices) {
    vertex.position *= radius;
  }

  return {std::move(vertices), std::move(indices)};
}

}  // namespace Simple3D::Internal
