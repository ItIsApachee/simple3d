#include <GLES3/gl3.h>
#include <simple3d/context/context.h>
#include <simple3d/gl/web/gles_shader.h>
#include <simple3d/gl/web/misc.h>
#include <simple3d/misc/error.h>

#include <cstdint>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <utility>

namespace Simple3D::Internal {

// GlesShaderBuilder& GlesShaderBuilder::VertexShaderSource(
//     const std::string& src) {
//   vertex_shader_src_ = src;
//   return *this;
// }

GlesShaderBuilder& GlesShaderBuilder::VertexShaderSource(std::string&& src) {
  vertex_shader_src_ = std::forward<std::string>(src);
  return *this;
}

// GlesShaderBuilder& GlesShaderBuilder::FragmentShaderSource(
//     const std::string& src) {
//   fragment_shader_src_ = src;
//   return *this;
// }

GlesShaderBuilder& GlesShaderBuilder::FragmentShaderSource(std::string&& src) {
  fragment_shader_src_ = std::forward<std::string>(src);
  return *this;
}

GlesShader GlesShaderBuilder::Build() {
  Error discarded_error;
  return Build(&discarded_error);
}

GlesShader GlesShaderBuilder::Build(Error* error) {
  if (App::GetCtxId() == 0) {
    return {};
  }

  // GlesShader result = GlesShader();
  if (!vertex_shader_src_) {
    *error = Error(ErrorType::kShaderCompilationFailed,
                   "shader compilation failed: no vertex shader");
    return {};
  }
  if (!fragment_shader_src_) {
    *error = Error(ErrorType::kShaderCompilationFailed,
                   "shader compilation failed: no fragment shader");
    return {};
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
    return {};
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
    return {};
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
    return {};
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  *error = Error(ErrorType::kOk);
  return GlesShader(shader_program, App::GetCtxId());
}

GLuint GlesShader::active_shader_id_{kGlesInvalidShader};

GlesShader::GlesShader(GLuint shader_id, std::int64_t ctx_id)
    : shader_id_{shader_id}, ctx_id_{ctx_id} {}

GlesShader::GlesShader(GlesShader&& other) : GlesShader{} {
  std::swap(shader_id_, other.shader_id_);
  std::swap(ctx_id_, other.ctx_id_);
}

GlesShader& GlesShader::operator=(GlesShader&& other) {
  if (&other == this) return *this;

  if (shader_id_ != kGlesInvalidShader && ctx_id_ == App::GetCtxId()) {
    glDeleteProgram(shader_id_);
  }

  shader_id_ = other.shader_id_;
  other.shader_id_ = kGlesInvalidShader;

  ctx_id_ = other.ctx_id_;
  other.ctx_id_ = 0;

  return *this;
}

GlesShader::~GlesShader() {
  if (shader_id_ != kGlesInvalidShader && ctx_id_ == App::GetCtxId())
    glDeleteProgram(shader_id_);
}

void GlesShader::Use() const {
  if (active_shader_id_ != shader_id_) {
    glUseProgram(shader_id_);
    active_shader_id_ = shader_id_;
  }
}

unsigned int GlesShader::GetID() const { return shader_id_; }

bool GlesShader::IsValid() const { return shader_id_ != 0; }

void GlesShader::Delete() {
  if (shader_id_ != kGlesInvalidShader && ctx_id_ == App::GetCtxId()) {
    glDeleteProgram(shader_id_);
    shader_id_ = 0;
  }
}

Error GlesShader::SetUniformMat4fv(const std::string& name,
                                   const glm::mat4& matrix) const {
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

Error GlesShader::SetUniform3fv(const std::string& name,
                                const glm::vec3& vec) const {
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

Error GlesShader::SetUniform1i(const std::string& name,
                               const GLint& val) const {
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
