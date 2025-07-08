#include "shader_program.h"

#include <simple3d/core/assert.h>
#include <simple3d/core/error.h>
#include <simple3d/core/error_code.h>

#include <glm/gtc/type_ptr.hpp>

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

TUniformLocationStore::TUniformLocationStore(std::unordered_map<std::string, GLint> uniformNameToLocationMapping, GLuint shaderHandle)
    : UniformNameToLocationMapping(std::move(uniformNameToLocationMapping))
#ifndef NDEBUG
    , ShaderHandle(shaderHandle)
#endif
{
#ifdef NDEBUG
    S3D_UNUSED(shaderHandle);
#endif
}

TUniformLocationStore::TUniformDescriptor TUniformLocationStore::GetUniformDescriptor(std::string uniformName) const
{
    auto it = UniformNameToLocationMapping.find(uniformName);
    if (it == UniformNameToLocationMapping.end()) [[unlikely]] {
        S3D_THROW("Unknown uniform name {}", uniformName);
    }
    S3D_VERIFY(it != UniformNameToLocationMapping.end());
    return TUniformDescriptor{
        .Location = it->second,
#ifndef NDEBUG
        .ShaderHandle = ShaderHandle,
#endif
    };
}

////////////////////////////////////////////////////////////////////////////////

GLuint TShaderProgram::ActiveHandle_{InvalidShaderHandle};

TShaderProgram::TShaderProgram(GLuint shaderID, TUniformLocationStore uniformLocationStore)
    : Handle_(shaderID)
    , UniformLocationStore_(std::move(uniformLocationStore))
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

TUniformDescriptor TShaderProgram::GetUniformDescriptor(std::string name) const
{
    return UniformLocationStore_.GetUniformDescriptor(name);
}

////////////////////////////////////////////////////////////////////////////////

TShaderProgramBuilder TShaderProgramBuilder::VertexShaderSource(std::string&& src) &&
{
    VertexShaderSource_ = std::forward<std::string>(src);
    return std::move(*this);
}

TShaderProgramBuilder TShaderProgramBuilder::FragmentShaderSource(std::string&& src) &&
{
    FragmentShaderSource_ = std::forward<std::string>(src);
    return std::move(*this);
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

    auto uniformLocationStore = TUniformLocationStore::Create(shaderProgramHandle, UniformNames_);

    return TShaderProgram(shaderProgramHandle, uniformLocationStore);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
