/** \file
 * \brief List of all classes declarations.
*/

#ifndef INCLUDE_SIMPLE3D_TYPES_H_
#define INCLUDE_SIMPLE3D_TYPES_H_

namespace Simple3D {



// context
class App;
class Window;
class IInputHandler;
class IWindowInputHandler;
// graphics
template <class M>
class ModelHandle;
class Scene;
class IRenderer;
class Camera;
class View;
// graphics/models
struct Cuboid;
class CuboidRenderer;
// shader
class Shader;
class GlesShaderBuilder;
// misc
enum class ErrorType;
struct Error;

namespace Internal {



class GlesShader;
class GlesShaderBuilder;
class ElementBufferObject;
class ElementBufferObjectBuilder;
class VertexBufferObject;
class VertexArrayObject;



}  // namespace Internal



}  // namespace Simple3D

#endif  // INCLUDE_SIMPLE3D_TYPES_H_
