#pragma once

#include "public.h"

#include <simple3d/core/error.h>
#include <simple3d/core/types.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <GLES3/gl3.h>

#include <optional>
#include <string>
#include <unordered_map>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

class TUniformLocationStore
{
public:
    struct TUniformDescriptor
    {
        GLint Location = -1;

#ifndef NDEBUG
        GLuint ShaderHandle = InvalidShaderHandle;
#endif
    };

    template <typename R>
    static TUniformLocationStore Create(const GLuint shaderProgramHandle, const R& uniformNames);

    TUniformLocationStore() = default;

    TUniformDescriptor GetUniformDescriptor(std::string uniformName) const;

private:
    std::unordered_map<std::string, GLint> UniformNameToLocationMapping;
#ifndef NDEBUG
    GLuint ShaderHandle = InvalidShaderHandle;
#endif

    TUniformLocationStore(std::unordered_map<std::string, GLint> uniformNameToLocationMapping, GLuint shaderHandle);
};

using TUniformDescriptor = TUniformLocationStore::TUniformDescriptor;

////////////////////////////////////////////////////////////////////////////////

// TODO(apachee): Use either ID or Handle, but not both.

class TShaderProgram
{
public:
    friend class TShaderProgramBuilder;
    friend void ResetActiveBindings();

    TShaderProgram() = default;
    TShaderProgram(const TShaderProgram&) = delete;
    TShaderProgram(TShaderProgram&&);

    TShaderProgram& operator=(const TShaderProgram&) = delete;
    TShaderProgram& operator=(TShaderProgram&&);

    ~TShaderProgram();

    //! Returns true if shader program was not already active.
    bool Use() const;

    unsigned int GetID() const;

    bool IsValid() const;

    void Delete();

    TUniformDescriptor GetUniformDescriptor(std::string name) const;

    template <typename T>
    void SetUniform(TUniformDescriptor uniformDescriptor, T&& value) const;

private:
    static GLuint ActiveHandle_;

    GLuint Handle_{InvalidShaderHandle};
    TUniformLocationStore UniformLocationStore_;

    TShaderProgram(GLuint shaderID, TUniformLocationStore uniformLocationStore);
};

////////////////////////////////////////////////////////////////////////////////

class TShaderProgramBuilder
{
public:
    TShaderProgramBuilder() = default;

    ~TShaderProgramBuilder() = default;

    TShaderProgramBuilder VertexShaderSource(std::string&& src) &&;
    TShaderProgramBuilder FragmentShaderSource(std::string&& src) &&;
    template <typename R>
    TShaderProgramBuilder SetUniformNames(const R& uniformNames) &&;

    TErrorOr<TShaderProgram> Build() &&;

protected:
    std::optional<std::string> VertexShaderSource_{};
    std::optional<std::string> FragmentShaderSource_{};
    std::vector<std::string> UniformNames_{};
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2

#define S3D_LIB_WEBGL2_SHADER_PROGRAM_INL_H
#include "shader_program-inl.h"
#undef S3D_LIB_WEBGL2_SHADER_PROGRAM_INL_H
