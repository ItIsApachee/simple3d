#include <simple3d/graphics/internal/gles_shader.h>

#include <string>
#include <sstream>
#include <optional>
#include <utility>

#include <glad/gles2.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <simple3d/misc/error.h>
#include <simple3d/graphics/internal/misc.h>

namespace Simple3D::Internal {



GlesShaderBuilder& GlesShaderBuilder::VertexShaderSource(const std::string& src) {
  vertex_shader_src_ = src;
  return *this;
}

GlesShaderBuilder& GlesShaderBuilder::VertexShaderSource(std::string&& src) {
  vertex_shader_src_ = src;
  return *this;
}

GlesShaderBuilder& GlesShaderBuilder::FragmentShaderSource(const std::string& src) {
  fragment_shader_src_ = src;
  return *this;
}

GlesShaderBuilder& GlesShaderBuilder::FragmentShaderSource(std::string&& src) {
  fragment_shader_src_ = src;
  return *this;
}

GlesShader GlesShaderBuilder::Build() {
  Error discarded_error;
  return Build(&discarded_error);
}

GlesShader GlesShaderBuilder::Build(Error* error) {
  // TODO(apachee): check if context is loaded
  GlesShader result = GlesShader();
  if (!vertex_shader_src_) {
    *error = Error(
      ErrorType::kShaderCompilationFailed,
      "shader compilation failed: no vertex shader");
    return result;
  }
  if (!fragment_shader_src_) {
    *error = Error(
      ErrorType::kShaderCompilationFailed,
      "shader compilation failed: no fragment shader");
    return result;
  }

  constexpr int INFO_LOG_MAX_SIZE = 512;
  int success;
  char info_log[INFO_LOG_MAX_SIZE];

  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* c_vertex_shader_src = vertex_shader_src_->c_str();
  glShaderSource(vertex_shader, 1, &c_vertex_shader_src, nullptr);
  glCompileShader(vertex_shader);

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, INFO_LOG_MAX_SIZE, nullptr, info_log);

    std::ostringstream error_out;
    error_out << "vertex shader coompilation failed: " << info_log;
    *error = Error(ErrorType::kShaderCompilationFailed, error_out.str());
    return result;
  }

  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* c_fragment_shader_src = fragment_shader_src_->c_str();
  glShaderSource(fragment_shader, 1, &c_fragment_shader_src, nullptr);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, INFO_LOG_MAX_SIZE, nullptr, info_log);

    std::ostringstream error_out;
    error_out << "vertex shader coompilation failed: " << info_log;
    *error = Error(ErrorType::kShaderCompilationFailed, error_out.str());
    return result;
  }

  unsigned int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, INFO_LOG_MAX_SIZE, nullptr, info_log);

    std::ostringstream error_out;
    error_out << "vertex shader coompilation failed: " << info_log;
    *error = Error(ErrorType::kShaderCompilationFailed, error_out.str());
    return result;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  result.shader_id_ = shader_program;
  *error = Error(ErrorType::kOk);
  return result;
}

GLuint GlesShader::active_shader_id_{kGlesInvalidShader};

GlesShader::GlesShader(GlesShader&& other) : shader_id_{other.shader_id_} {
  other.shader_id_ = kGlesInvalidShader;
}

GlesShader& GlesShader::operator=(GlesShader&& other) {
  if (&other == this)
    return *this;
  
  if (shader_id_ != kGlesInvalidShader) {
    GlesShader tmp{std::move(*this)};
  }  // tmp is destroyed

  shader_id_ = other.shader_id_;
  other.shader_id_ = kGlesInvalidShader;

  return *this;
}

void GlesShader::Use() const {
  if (active_shader_id_ != shader_id_) {
    glUseProgram(shader_id_);
    active_shader_id_ = shader_id_;
  }
}

unsigned int GlesShader::GetID() const {
  return shader_id_;
}

bool GlesShader::IsValid() const {
  return shader_id_ != 0;
}

void GlesShader::Delete() {
  if (IsValid()) {
    glDeleteProgram(shader_id_);
    shader_id_ = 0;
  }
}

Error GlesShader::SetUniformMat4fv(
    const std::string& name, const glm::mat4& matrix) const {
  int location = glGetUniformLocation(shader_id_, name.c_str());
  if (location == -1) {
    std::string desc = "uniform not found, name=\"";
    desc += name;
    desc += "\"";
    return Error(ErrorType::kUniformNotFound, desc);
  }
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  return Error(ErrorType::kOk);
}

Error GlesShader::SetUniform3fv(
    const std::string& name, const glm::vec3& vec) const {
  int location = glGetUniformLocation(shader_id_, name.c_str());
  if (location == -1) {
    std::string desc = "uniform not found, name=\"";
    desc += name;
    desc += "\"";
    return Error(ErrorType::kUniformNotFound, desc);
  }
  glUniform3fv(location, 1, glm::value_ptr(vec));
  return Error(ErrorType::kOk);
}

Error GlesShader::SetUniform1i(
    const std::string& name, const GLint& val) const {
  int location = glGetUniformLocation(shader_id_, name.c_str());
  if (location == -1) {
    std::string desc = "uniform not found, name=\"";
    desc += name;
    desc += "\"";
    return Error(ErrorType::kUniformNotFound, desc);
  }
  glUniform1i(location, val);
  return Error(ErrorType::kOk);
}



}  // namespace Simple3D::Internal
