#ifndef SIMPLE3D_GRAPHICS_VIEW_H
#define SIMPLE3D_GRAPHICS_VIEW_H

#include <simple3d/types.h>
#include <simple3d/graphics/scene.h>

namespace Simple3D {



/**
 * \class View simple3d/graphics/view.h
 * \brief Class representing object you can draw to.
*/
class View {
public:
    explicit View(); // create framebuffer
    View(const View&) = delete;
    View(View&&) = default;
    View& operator=(const View&) = delete;
    View& operator=(View&&) = default;
    ~View(); // delete framebuffer if not 0

    void Draw(const Scene& scene);
private:
    unsigned int framebuffer_{0};
};



}

#endif