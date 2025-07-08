#ifndef S3D_LIB_WEBGL2_SHADER_PROGRAM_INL_H
#error This file should not be included directly, use .h version
#include "shader_program.h"
#endif

#include "helpers.h"

#include <simple3d/core/assert.h>

#include <GLES3/gl3.h>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

template <typename R>
TUniformLocationStore TUniformLocationStore::Create(const GLuint shaderProgramHandle, const R& uniformNames)
{
    std::unordered_map<std::string, GLint> uniformNameToLocationMapping;

    uniformNameToLocationMapping.reserve(std::ssize(uniformNames));

    for (const auto& uniformName : uniformNames) {
        static_assert(std::is_same_v<std::remove_cvref_t<decltype(uniformName)>, std::string>);

        auto location = glGetUniformLocation(shaderProgramHandle, uniformName.c_str());
        if (location == -1) [[unlikely]] {
            S3D_THROW("Uniform name {} not found in shader", uniformName);
        }
        S3D_VERIFY(location != -1);

        bool inserted = uniformNameToLocationMapping.insert(std::pair{uniformName, location}).second;
        if (!inserted) [[unlikely]] {
            S3D_THROW("Duplicate uniform name {}", uniformName);
        }
    }

    return TUniformLocationStore(std::move(uniformNameToLocationMapping), shaderProgramHandle);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
void TShaderProgram::SetUniform(TUniformDescriptor uniformDescriptor, T&& value) const
{
    S3D_VERIFY(ActiveHandle_ == GetID());
#ifndef NDEBUG
    S3D_ASSERT(uniformDescriptor.ShaderHandle == GetID());
#endif

    auto location = uniformDescriptor.Location;
    SetUniform(location, std::forward<T>(value));
}

////////////////////////////////////////////////////////////////////////////////

template <typename R>
TShaderProgramBuilder TShaderProgramBuilder::SetUniformNames(const R& uniformNames) &&
{
    UniformNames_ = std::vector<std::string>(begin(uniformNames), end(uniformNames));
    return std::move(*this);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
