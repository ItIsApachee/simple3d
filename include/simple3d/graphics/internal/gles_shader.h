/** \dir
 * \brief GlesShader component.
*/
/** \file
 * \brief Managing shaders.
*/

#ifndef INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_SHADER_H_
#define INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_SHADER_H_

#include <string>
#include <optional>

#include <glad/gles2.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <simple3d/types.h>
#include <simple3d/misc/error.h>
#include <simple3d/graphics/internal/misc.h>

namespace Simple3D::Internal {



// TODO(apachee): change name to GlesShader
// TODO(apachee): consider adding support for compute shader, or add
// ComputeShader class
/** \class GlesShader simple3d/shader/shader.h
 * \brief An abstraction over GLES shader.
*/
class GlesShader {
 public:
  friend GlesShaderBuilder;

  GlesShader() = default;
  GlesShader(const GlesShader&) = delete;
  GlesShader(GlesShader&&);

  GlesShader& operator=(const GlesShader&) = delete;
  GlesShader& operator=(GlesShader&&);

  // FIXME: delete shader on destruction
  ~GlesShader() = default;

  void Use() const;
  unsigned int GetID() const;
  bool IsValid() const;
  void Delete();
  // TODO(apachee): consider adding methods to modify uniforms

  Error SetUniformMat4fv(const std::string& name, const glm::mat4& matrix) const;
  Error SetUniform3fv(const std::string& name, const glm::vec3& vec) const;
  Error SetUniform1i(const std::string& name, const GLint& val) const;

 private:
  static GLuint active_shader_id_;

  GLuint shader_id_{kGlesInvalidShader};
};

/** \class GlesShaderBuilder simple3d/shader/shader.h
 * \brief Helper class for constructing Simple3D::GlesShader.
*/
class GlesShaderBuilder {
// FIXME(apachee): remove GlesShaderBuilder (no reason for a builder)
 public:
  GlesShaderBuilder() = default;
  GlesShaderBuilder(const GlesShaderBuilder&) = default;
  GlesShaderBuilder(GlesShaderBuilder&&) = default;

  GlesShaderBuilder& operator=(const GlesShaderBuilder&) = default;
  GlesShaderBuilder& operator=(GlesShaderBuilder&&) = default;

  ~GlesShaderBuilder() = default;

  GlesShader Build();
  GlesShader Build(Error* error);
  GlesShaderBuilder& VertexShaderSource(const std::string& src);
  GlesShaderBuilder& VertexShaderSource(std::string&& src);
  GlesShaderBuilder& FragmentShaderSource(const std::string& src);
  GlesShaderBuilder& FragmentShaderSource(std::string&& src);
 private:
  std::optional<std::string> vertex_shader_src_{};
  std::optional<std::string> fragment_shader_src_{};
};



}  // namespace Simple3D::Internal

#endif  // INCLUDE_SIMPLE3D_GRAPHICS_INTERNAL_SHADER_H_
