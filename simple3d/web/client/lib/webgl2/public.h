#pragma once

#include <GLES3/gl3.h>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

// FIXME(apachee): Fix consts naming.

constexpr GLuint InvalidBufferHandle = 0;

constexpr GLuint InvalidShaderHandle = 0;

// only used as default value for VAO field, and
// shouldn't be used for any other reason whatsoever
constexpr GLenum DefaultVaoHandle = 0;

constexpr GLenum kDefaultUsage = GL_STATIC_DRAW;

////////////////////////////////////////////////////////////////////////////////

class TShaderProgram;
class TShaderBuilder;
class TEbo;
class TEboBuilder;
class TVbo;
class TVao;

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
