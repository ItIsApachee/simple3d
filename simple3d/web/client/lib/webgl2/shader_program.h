#pragma once

#include "public.h"

#include <simple3d/core/error.h>
#include <simple3d/core/types.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <GLES3/gl3.h>

#include <optional>
#include <string>
#include <memory>
#include <unordered_map>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

// // TODO(apachee): Add TUniformID for generating list of all uniform names in more complex cases,
// // such as structs and arrays.

// class TSimpleUniformID
// {
//     std::string_view GetID() const;
// };

// ////////////////////////////////////////////////////////////////////////////////

// template <typename TKey, typename TValue>
// class TUniformIDMapKeyTraits
// { };

// template <typename TValue>
// class TUniformIDMapKeyTraits<ui32, TValue>
// {
//     using TContainer = std::vector<TValue>;
// };

// template <typename TValue>
// class TUniformIDMapKeyTraits<std::string, TValue>
// {
//     using TContainer = std::unordered_map<std::string, TValue>;
// };

// ////////////////////////////////////////////////////////////////////////////////

// //! Holder of all UniformIDs for some complex uniform.
// //! Owns all the IDs of a uniform.
// class TUniformIDHolder
//     : public std::enable_shared_from_this<TUniformIDHolder>
// {
// public:
//     TUniformIDHolder() = default;

//     void RegisterID(std::string id);

// private:
//     std::vector<std::string> UniformIDs;
// };

// ////////////////////////////////////////////////////////////////////////////////

// template <typename TKey, typename TValue, typename TContainer = TUniformIDMapKeyTraits<TKey, TValue>>
// class TUniformIDMap
// {
// public:
//     TUniformIDMap(TContainer keyToValueMapping, TUniformIDHolder uniformIDHolder = {});

//     virtual ~TUniformIDMap() = default;

//     std::vector<std::string_view> GetIDs() const;

//     TValue operator[]

// protected:
//     TContainer KeyToValueMapping_;

//     TUniformIDHolderPtr UniformIDHolder_;
// };

////////////////////////////////////////////////////////////////////////////////

class TUniformLocationStore
{
public:
    // FIXME(apachee): Had to remove const, but maybe it can be kept somehow?
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
    const std::unordered_map<std::string, GLint> UniformNameToLocationMapping;
#ifndef NDEBUG
    const GLuint ShaderHandle = InvalidShaderHandle;
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

    void Use() const;

    unsigned int GetID() const;

    bool IsValid() const;

    void Delete();

    TUniformDescriptor GetUniformDescriptor(std::string name) const;

    template <typename T>
    void SetUniform(TUniformDescriptor uniformDescriptor, T&& value) const;

private:
    static GLuint ActiveHandle_;

    GLuint Handle_{InvalidShaderHandle};
    const TUniformLocationStore UniformLocationStore_;

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
