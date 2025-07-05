#pragma once

#include "public.h"

#include <GLES3/gl3.h>

#include <glm/glm.hpp>

#include <memory>
#include <type_traits>
#include <unordered_set>

namespace NSimple3D::NGraphics {

////////////////////////////////////////////////////////////////////////////////

struct TDirectionalLight
{
    glm::vec3 direction = glm::vec3(0.0f);
    glm::vec3 diffuse = glm::vec3(0.0f);
    glm::vec3 specular = glm::vec3(0.0f);
};

////////////////////////////////////////////////////////////////////////////////

struct IShader
    : public std::enable_shared_from_this<IShader>
{
    virtual void SetView(const glm::mat4& view) = 0;
    virtual void SetProj(const glm::mat4& projection) = 0;
    virtual void SetViewPos(const glm::vec3& pos) = 0;
    virtual void SetDirectionalLights(
        const std::unordered_set<std::shared_ptr<TDirectionalLight>>& lights) = 0;
    virtual void SetAmbientLight(const glm::vec3& color) = 0;
    virtual void Use() const = 0;

    virtual ~IShader() = default;
};

////////////////////////////////////////////////////////////////////////////////

IShaderPtr CreateModelShader();
// TODO(apachee): Add second type of shader.
// IShaderPtr CreateTexturedModelShader();

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

////////////////////////////////////////////////////////////////////////////////

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

}  // namespace NSimple3D::NGraphics
