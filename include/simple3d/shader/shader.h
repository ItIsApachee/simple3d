/** \dir
 * \brief Shader component.
*/
/** \file
 * \brief Managing shaders.
*/

#ifndef SIMPLE3D_SHADER_SHADER_H
#define SIMPLE3D_SHADER_SHADER_H

#include <string>
#include <optional>

#include <glad/gles2.h>
#include <glm/mat4x4.hpp>

#include <simple3d/types.h>
#include <simple3d/misc/error.h>

namespace Simple3D {



class Shader;
class ShaderBuilder;

// TODO: consider adding support for compute shader, or add ComputeShader class
class Shader {
public:
    friend ShaderBuilder;

    Shader() = default;
    Shader(const Shader&) = delete;
    Shader(Shader&&) = default;

    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = default;

    ~Shader() = default;

    void Use(GladGLES2Context* gl) const;
    unsigned int GetID() const;
    bool IsValid() const;
    void Delete(GladGLES2Context* gl);
    // TODO: consider adding methods to modify uniforms

    Error SetUniformMat4fv(GladGLES2Context* gl, const std::string& name, const glm::mat4& matrix);
private:
    unsigned int shader_id_{0};
};

class ShaderBuilder {
public:
    ShaderBuilder() = default;
    ShaderBuilder(const ShaderBuilder&) = default;
    ShaderBuilder(ShaderBuilder&&) = default;

    ShaderBuilder& operator=(const ShaderBuilder&) = default;
    ShaderBuilder& operator=(ShaderBuilder&&) = default;

    ~ShaderBuilder() = default;

    Shader Build(GladGLES2Context* gl_);
    Shader Build(GladGLES2Context* gl_, Error& error);
    ShaderBuilder& VertexShaderSource(const std::string& src);
    ShaderBuilder& VertexShaderSource(std::string&& src);
    ShaderBuilder& FragmentShaderSource(const std::string& src);
    ShaderBuilder& FragmentShaderSource(std::string&& src);
private:
    std::optional<std::string> vertex_shader_src_{};
    std::optional<std::string> fragment_shader_src_{};
};



}

#endif