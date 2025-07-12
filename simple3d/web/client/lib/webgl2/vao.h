#pragma once

#include "public.h"

#include <GLES3/gl3.h>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

class TVao
{
public:
    friend void ResetActiveBindings();

    static TVao Generate();

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
    //! Currently active VAO.
    static GLuint ActiveHandle_;

    GLuint Handle_{DefaultVaoHandle};
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
