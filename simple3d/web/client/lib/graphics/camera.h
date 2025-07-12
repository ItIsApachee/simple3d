#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <GLES3/gl3.h>

namespace NSimple3D::NGraphics {

// TODO(apachee): Add more flexibility in terms of the variety of cams
// and their configurations, e.g. cam for 2D rendering.

////////////////////////////////////////////////////////////////////////////////

struct TCameraState3D
{
    glm::vec3 Pos{0.0f};
    GLfloat Roll{0.0f};
    GLfloat Pitch{0.0f};
    GLfloat Yaw{0.0f};
    GLfloat Fov{45.0f};
    GLfloat AspectRatio{16.0f / 9.0f};

    glm::mat4 GetView() const;
    glm::mat4 GetProj() const;
    glm::vec3 GetViewPos() const;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NGraphics
