/** \file
 * \brief List of all classes declarations.
*/

#ifndef SIMPLE3D_TYPES_H
#define SIMPLE3D_TYPES_H

namespace Simple3D {



// context
class Context;
class Window;
// graphics
template <class M>
class Model;
class Scene;
class IRenderer;
class Camera;
class View;
// graphics/models
struct Cuboid;
class CuboidRenderer;
// shader
class Shader;
class ShaderBuilder;
// misc
enum class ErrorType;
struct Error;



}

#endif