#pragma once

#include <GLES3/gl3.h>

#include <glm/glm.hpp>

#include <type_traits>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

// XXX(apachee): Maybe provide wrappers that hide glm usage?

struct Vertex
{
    // TODO(apachee): (outdated) move to ModelShader

    static void BindAttributes();

    glm::vec3 Pos{0.f, 0.f, 0.f};
    glm::vec3 Normal{1.f, 0.f, 0.f};
};

static_assert(std::is_standard_layout_v<Vertex>);

struct TexturedVertex
{
    // TODO(apachee): (outdated) add more customization
    // TODO(apachee): (outdated) move to TexturedModelShader

    static void BindAttributes();

    glm::vec3 Pos{0.f, 0.f, 0.f};
    glm::vec3 Normal{1.f, 0.f, 0.f};
    glm::vec2 TexCoords{0.f, 0.f};
};

static_assert(std::is_standard_layout_v<TexturedVertex>);

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
