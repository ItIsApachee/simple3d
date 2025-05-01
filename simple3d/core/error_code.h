#pragma once

namespace NSimple3D {

////////////////////////////////////////////////////////////////////////////////

// XXX(apachee): Simplest approximation for the time being.

enum struct EErrorCode : int
{
    OK = 0,
    Generic = 1,
    // XXX(apachee): Not used in new code.
    // InitFailed = 2,
    // AlreadyInitialized = 3,
    WindowCreationFailed = 4,
    WebGL2ShaderCompilationFailed = 5,
    WebGL2UniformNotFound = 6,
    WebGL2UnitializedShader = 7,
};


////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D
