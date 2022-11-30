#include <simple3d/graphics/internal/shader.h>

#include <string>
#include <sstream>
#include <optional>
#include <utility>

#include <glad/gles2.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <simple3d/misc/error.h>

namespace Simple3D {



ShaderBuilder& ShaderBuilder::VertexShaderSource(const std::string& src) {
  vertex_shader_src_ = src;
  return *this;
}

ShaderBuilder& ShaderBuilder::VertexShaderSource(std::string&& src) {
  vertex_shader_src_ = src;
  return *this;
}

ShaderBuilder& ShaderBuilder::FragmentShaderSource(const std::string& src) {
  fragment_shader_src_ = src;
  return *this;
}

ShaderBuilder& ShaderBuilder::FragmentShaderSource(std::string&& src) {
  fragment_shader_src_ = src;
  return *this;
}

Shader ShaderBuilder::Build() {
  Error discarded_error;
  return Build(&discarded_error);
}

Shader ShaderBuilder::Build(Error* error) {
  // TODO(apachee): check if context is loaded
  Shader result = Shader();
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

void Shader::Use() const {
  glUseProgram(shader_id_);
}

unsigned int Shader::GetID() const {
  return shader_id_;
}

bool Shader::IsValid() const {
  return shader_id_ != 0;
}

void Shader::Delete() {
  if (IsValid()) {
    glDeleteProgram(shader_id_);
    shader_id_ = 0;
  }
}

Error Shader::SetUniformMat4fv(
    const std::string& name, const glm::mat4& matrix) {
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



}  // namespace Simple3D
