#pragma once

#include <emscripten.h>

#include <memory>

namespace NApachee::NSimple3D::NGLib {

////////////////////////////////////////////////////////////////////////////////

//! Initializes WebGL context and sets main loop.
void Start();

//! Initializes renderer and provides with callback for Emscripten.
void SetMainLoop();

////////////////////////////////////////////////////////////////////////////////

}  // namespace NApachee::NSimple3D::NGLib
