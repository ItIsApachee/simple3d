#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <array>

// FIXME(apachee): Fix the mess with namespaces.

namespace NSimple3D::NGraphics {

////////////////////////////////////////////////////////////////////////////////

// FIXME(apachee): For now it is assumed that server and client both run with little endian.

struct TTriangle
{
    struct TVertex
    {
        glm::vec3 Pos = glm::vec3(0.0f);
        glm::vec3 Diffuse = glm::vec3(0.0f);
        glm::vec3 Specular = glm::vec3(0.0f);
    };

    glm::mat4 Model = glm::mat4(1.0);
    std::array<TVertex, 3> Vertices{};

    // i-th component is shininess for i-th vertex.
    glm::vec3 Shininess = glm::vec3(16.0f);
    glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 1.0f);
};

using TVertex = TTriangle::TVertex;


////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NGraphics
