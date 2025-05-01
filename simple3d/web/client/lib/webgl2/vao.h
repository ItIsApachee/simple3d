#pragma once

#include "public.h"

#include <GLES3/gl3.h>

#include <cstdint>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

class TVao
{
public:
    friend void ResetActiveBindings();

    // FIXME(apachee): Remove some explicit RO5 methods

    TVao();
    TVao(TVao&&);
    TVao& operator=(TVao&&);
    TVao(const TVao&) = delete;
    TVao& operator=(const TVao&) = delete;
    ~TVao();

    void BindEbo(const TEbo& ebo) const;

    void Bind() const;

    static void Unbind();

private:
    // NB(apachee): Currently active VAO.
    static GLuint ActiveHandle_;

    GLuint Handle_{DefaultVaoHandle};
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
