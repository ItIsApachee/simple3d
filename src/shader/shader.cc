#include <simple3d/shader/shader.h>

#include <string>
#include <sstream>
#include <optional>
#include <utility>

#include <glad/gles2.h>

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

Shader ShaderBuilder::Build(GladGLES2Context* gl_) {
    Error discarded_error;
    return Build(gl_, discarded_error);
}

Shader ShaderBuilder::Build(GladGLES2Context* gl_, Error& error) {
    Shader result = Shader();
    if (!vertex_shader_src_) {
        error = Error(ErrorType::kShaderCompilationFailed, "shader compilation failed: no vertex shader");
        return result;
    }
    if (!fragment_shader_src_) {
        error = Error(ErrorType::kShaderCompilationFailed, "shader compilation failed: no fragment shader");
        return result;
    }

    GladGLES2Context& gl = *gl_;

    constexpr int INFO_LOG_MAX_SIZE = 512;
    int success;
    char info_log[INFO_LOG_MAX_SIZE];

    unsigned int vertex_shader = gl.CreateShader(GL_VERTEX_SHADER);
    const char* c_vertex_shader_src = vertex_shader_src_->c_str();
    gl.ShaderSource(vertex_shader, 1, &c_vertex_shader_src, nullptr);
    gl.CompileShader(vertex_shader);

    gl.GetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        gl.GetShaderInfoLog(vertex_shader, INFO_LOG_MAX_SIZE, nullptr, info_log);

        std::ostringstream error_out;
        error_out << "vertex shader coompilation failed: " << info_log;
        error = Error(ErrorType::kShaderCompilationFailed, error_out.str());
        return result;
    }

    unsigned int fragment_shader = gl.CreateShader(GL_FRAGMENT_SHADER);
    const char* c_fragment_shader_src = fragment_shader_src_->c_str();
    gl.ShaderSource(fragment_shader, 1, &c_fragment_shader_src, nullptr);
    gl.CompileShader(fragment_shader);

    gl.GetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        gl.GetShaderInfoLog(fragment_shader, INFO_LOG_MAX_SIZE, nullptr, info_log);

        std::ostringstream error_out;
        error_out << "vertex shader coompilation failed: " << info_log;
        error = Error(ErrorType::kShaderCompilationFailed, error_out.str());
        return result;
    }

    unsigned int shader_program = gl.CreateProgram();
    gl.AttachShader(shader_program, vertex_shader);
    gl.AttachShader(shader_program, fragment_shader);
    gl.LinkProgram(shader_program);

    gl.GetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        gl.GetProgramInfoLog(shader_program, INFO_LOG_MAX_SIZE, nullptr, info_log);

        std::ostringstream error_out;
        error_out << "vertex shader coompilation failed: " << info_log;
        error = Error(ErrorType::kShaderCompilationFailed, error_out.str());
        return result;
    }

    gl.DeleteShader(vertex_shader);
    gl.DeleteShader(fragment_shader);

    result.shader_id_ = shader_program;
    error = Error(ErrorType::kOk);
    return result;
}

void Shader::Use(GladGLES2Context* gl) const {
    gl->UseProgram(shader_id_);
}

unsigned int Shader::GetID() const {
    return shader_id_;
}

bool Shader::IsValid() const {
    return shader_id_ != 0;
}

void Shader::Delete(GladGLES2Context* gl) {
    if (IsValid()) {
        gl->DeleteProgram(shader_id_);
        shader_id_ = 0;
    }
}



}