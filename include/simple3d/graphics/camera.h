/** \file
 * \brief Camera manipulation.
*/

#ifndef INCLUDE_SIMPLE3D_GRAPHICS_CAMERA_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_CAMERA_H_

#include <glad/gles2.h>
#include <glm/mat4x4.hpp>

#include <simple3d/types.h>

namespace Simple3D {



class ICamera {
 public:
  ICamera() = default;
  virtual ~ICamera() = default;
  
  virtual glm::mat4 GetModel() = 0;
  virtual glm::mat4 GetProjection() = 0;
};

/** \class Camera simple3d/graphics/camera.h
 * \brief An object representing observer position, and observer field of view on the scene.
 */
class Camera : public ICamera {
 public:
  Camera() = default;

  GLfloat x{0.0f};
  GLfloat y{0.0f};
  GLfloat z{0.0f};
  GLfloat roll{0.0f};
  GLfloat pitch{0.0f};
  GLfloat yaw{0.0f};

  glm::mat4 GetModel() override;
  glm::mat4 GetProjection() override;
};



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_CAMERA_H_
