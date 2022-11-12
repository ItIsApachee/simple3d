/** \file
 * \brief Simple3D::Drawable interface.
*/

#ifndef SIMPLE3D_GRAPHICS_DRAWABLE_H
#define SIMPLE3D_GRAPHICS_DRAWABLE_H

#include <simple3d/types.h>

namespace Simple3D {



/** \interface Drawable simple3d/graphics/drawable.h
 * \brief Interface for objects, which are drawable on the scene.
*/
class Drawable {
public:
    Drawable() = default;

    virtual ~Drawable() = default;

    
};



}

#endif