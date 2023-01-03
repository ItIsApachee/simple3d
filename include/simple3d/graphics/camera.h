/** \file
 * \brief Camera manipulation.
*/

#ifndef INCLUDE_SIMPLE3D_GRAPHICS_CAMERA_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_CAMERA_H_

#include <glad/gles2.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <simple3d/types.h>

namespace Simple3D {



class ICamera {
 public:
  ICamera() = default;
  virtual ~ICamera() = default;
  
  // return identity matrix by default
  virtual glm::mat4 GetView();
  virtual glm::mat4 GetProj();
  virtual glm::vec3 GetViewPos();
};

/** \class Camera simple3d/graphics/camera.h
 * \brief An object representing observer position, and observer field of view on the scene.
 */
class Camera : public ICamera {
 public:
  Camera() = default;
  ~Camera() override = default;

  GLfloat x{0.0f};
  GLfloat y{0.0f};
  GLfloat z{0.0f};
  GLfloat roll{0.0f};
  GLfloat pitch{0.0f};
  GLfloat yaw{0.0f};
  GLfloat fov{45.0f};
  GLfloat aspect_ratio{16.0f/9.0f};

  glm::mat4 GetView() override;
  glm::mat4 GetProj() override;
  glm::vec3 GetViewPos() override;
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_CAMERA_H_
