#include "shader_program.h"

#include <glm/gtc/type_ptr.hpp>
#include <simple3d/core/error_code.h>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

namespace {

////////////////////////////////////////////////////////////////////////////////

enum struct EShaderType : GLenum
{
    VertexShader = GL_VERTEX_SHADER,
    FragmentShader = GL_FRAGMENT_SHADER,
};

////////////////////////////////////////////////////////////////////////////////

GLuint CreateShader(EShaderType type)
{
    return glCreateShader(static_cast<GLenum>(type));
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace

////////////////////////////////////////////////////////////////////////////////

GLuint TShaderProgram::ActiveHandle_{InvalidShaderHandle};

TShaderProgram::TShaderProgram(GLuint shader_id)
    : Handle_(shader_id)
{ }

TShaderProgram::TShaderProgram(TShaderProgram&& other)
    : TShaderProgram()
{
    std::swap(Handle_, other.Handle_);
}

TShaderProgram& TShaderProgram::operator=(TShaderProgram&& other)
{
    if (&other == this) {
        return *this;
    }

    // Clear state
    {
        TShaderProgram shader = std::move(*this);
    }

    // Swap with cleared state
    std::swap(Handle_, other.Handle_);

    return *this;
}

TShaderProgram::~TShaderProgram()
{
    if (Handle_ != InvalidShaderHandle) {
        glDeleteProgram(Handle_);
    }
}

void TShaderProgram::Use() const
{
    if (ActiveHandle_ != Handle_) {
        glUseProgram(Handle_);
        ActiveHandle_ = Handle_;
    }
}

unsigned int TShaderProgram::GetID() const
{
    return Handle_;
}

bool TShaderProgram::IsValid() const
{
    return Handle_ != 0;
}

void TShaderProgram::Delete() {
    if (Handle_ != InvalidShaderHandle) {
        glDeleteProgram(Handle_);
        Handle_ = 0;
    }
}

// FIXME(apachee): Cache uniform locations (needed to improve WebGL2 performance)

TError TShaderProgram::SetUniformMat4fv(
    const std::string& name,
    const glm::mat4& matrix) const
{
    int location = glGetUniformLocation(Handle_, name.c_str());
    if (location == -1) {
        return TError(EErrorCode::WebGL2UniformNotFound, "Uniform with name {} not found", name);
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    return TError();
}

TError TShaderProgram::SetUniform3fv(
    const std::string& name,
    const glm::vec3& vec) const
{
    int location = glGetUniformLocation(Handle_, name.c_str());
    if (location == -1) {
        return TError(EErrorCode::WebGL2UniformNotFound, "Uniform with name {} not found", name);
    }
    glUniform3fv(location, 1, glm::value_ptr(vec));
    return TError();
}

TError TShaderProgram::SetUniform1i(
    const std::string& name,
    const GLint& val) const
{
    int location = glGetUniformLocation(Handle_, name.c_str());
    if (location == -1) {
        return TError(EErrorCode::WebGL2UniformNotFound, "Uniform with name {} not found", name);
    }
    glUniform1i(location, val);
    return TError();
}

////////////////////////////////////////////////////////////////////////////////

TShaderProgramBuilder TShaderProgramBuilder::VertexShaderSource(std::string&& src) &&
{
    VertexShaderSource_ = std::forward<std::string>(src);
    return *this;
}

TShaderProgramBuilder TShaderProgramBuilder::FragmentShaderSource(std::string&& src) &&
{
    FragmentShaderSource_ = std::forward<std::string>(src);
    return *this;
}


TErrorOr<TShaderProgram> TShaderProgramBuilder::Build() &&
{
    if (!VertexShaderSource_) {
        return TError(EErrorCode::WebGL2ShaderCompilationFailed, "Shader compilation failed: no vertex shader");
    }
    if (!FragmentShaderSource_) {
        return TError(EErrorCode::WebGL2ShaderCompilationFailed, "Shader compilation failed: no fragment shader");
    }

    constexpr int InfoLogBufferSize = 512;
    int success;
    char infoLogBuffer[InfoLogBufferSize];

    auto vertexShaderHandle = CreateShader(EShaderType::VertexShader);
    const char* rawVertexShaderSource = VertexShaderSource_->c_str();
    glShaderSource(vertexShaderHandle, 1, &rawVertexShaderSource, nullptr);
    glCompileShader(vertexShaderHandle);

    glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShaderHandle, InfoLogBufferSize, nullptr, infoLogBuffer);

        return TError(EErrorCode::WebGL2ShaderCompilationFailed, "Vertex shader compilation failed: {}", infoLogBuffer);
    }

    auto fragmentShaderHandle = CreateShader(EShaderType::FragmentShader);
    const char* rawFragmentShaderSource = FragmentShaderSource_->c_str();
    glShaderSource(fragmentShaderHandle, 1, &rawFragmentShaderSource, nullptr);
    glCompileShader(fragmentShaderHandle);

    glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderHandle, InfoLogBufferSize, nullptr, infoLogBuffer);

        return TError(EErrorCode::WebGL2ShaderCompilationFailed, "Fragment shader compilation failed: {}", infoLogBuffer);
    }

    auto shaderProgramHandle = glCreateProgram();
    glAttachShader(shaderProgramHandle, vertexShaderHandle);
    glAttachShader(shaderProgramHandle, fragmentShaderHandle);
    glLinkProgram(shaderProgramHandle);

    glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramHandle, InfoLogBufferSize, nullptr, infoLogBuffer);

        return TError(EErrorCode::WebGL2ShaderCompilationFailed, "Shader program compilation failed: {}", infoLogBuffer);
    }

    glDeleteShader(vertexShaderHandle);
    glDeleteShader(fragmentShaderHandle);

    return TShaderProgram(shaderProgramHandle);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
