#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace NSimple3D::NGraphics {

////////////////////////////////////////////////////////////////////////////////

glm::mat4 TCameraState3D::GetView() const
{
    glm::mat4 view(1.0f);
    view = glm::rotate(view, Roll, glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::rotate(view, Pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, Yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, -Pos);
    return view;
}

glm::mat4 TCameraState3D::GetProj() const
{
    glm::mat4 proj = glm::perspective(glm::radians(Fov), AspectRatio, 0.1f, 1000.0f);
    return proj;
}

glm::vec3 TCameraState3D::GetViewPos() const
{
    return Pos;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NGraphics