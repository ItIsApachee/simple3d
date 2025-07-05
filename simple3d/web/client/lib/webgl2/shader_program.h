#pragma once

#include "public.h"

#include <simple3d/core/error.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <GLES3/gl3.h>

#include <cstdint>
#include <optional>
#include <string>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

class TShaderProgram
{
public:
    friend TShaderProgramBuilder;
    friend void ResetActiveBindings();

    // Remove some explicit RO5 methods.

    TShaderProgram() = default;
    TShaderProgram(const TShaderProgram&) = delete;
    TShaderProgram(TShaderProgram&&);

    TShaderProgram& operator=(const TShaderProgram&) = delete;
    TShaderProgram& operator=(TShaderProgram&&);

    ~TShaderProgram();

    void Use() const;

    unsigned int GetID() const;

    bool IsValid() const;

    void Delete();

    TError SetUniformMat4fv(
        const std::string &name,
        const glm::mat4 &matrix) const;

    TError SetUniform3fv(const std::string& uniformName, const glm::vec3& value) const;

    TError SetUniform1i(const std::string& uniformName, const GLint& value) const;

private:
    static GLuint ActiveHandle_;

    TShaderProgram(GLuint shader_id);

    GLuint Handle_{InvalidShaderHandle};
};

////////////////////////////////////////////////////////////////////////////////

// FIXME(apachee): (outdated) Remove GlesShaderBuilder (no reason for a builder).
class TShaderProgramBuilder
{
public:
    TShaderProgramBuilder() = default;

    ~TShaderProgramBuilder() = default;

    TErrorOr<TShaderProgram> Build() &&;

    TShaderProgramBuilder VertexShaderSource(std::string&& src) &&;

    TShaderProgramBuilder FragmentShaderSource(std::string&& src) &&;

private:
    std::optional<std::string> VertexShaderSource_{};
    std::optional<std::string> FragmentShaderSource_{};
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
