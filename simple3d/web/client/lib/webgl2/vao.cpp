#include "vao.h"

#include "ebo.h"

#include <simple3d/web/client/lib/webgl2/helpers.h>
#include <utility>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

GLuint TVao::ActiveHandle_{DefaultVaoHandle};

////////////////////////////////////////////////////////////////////////////////

TVao TVao::Generate()
{
    TVao vao;
    glGenVertexArrays(1, &vao.Handle_);
    return vao;
}

TVao::TVao()
{
}

TVao::TVao(TVao&& other)
    : Handle_{DefaultVaoHandle}
{
    std::swap(Handle_, other.Handle_);
}

TVao& TVao::operator=(TVao&& other)
{
    if (&other == this) return *this;

    // Clear state
    {
        TVao vao = std::move(*this);
    }

    // Swap with cleared state
    std::swap(Handle_, other.Handle_);

    return *this;
}

TVao::~TVao()
{
    if (Handle_ != DefaultVaoHandle) {
        glDeleteVertexArrays(1, &Handle_);
    }
}

void TVao::BindEbo(const TEbo& ebo) const
{
    Bind();
    BindBuffer(EBufferType::ElementArrayBuffer, ebo.GetHandle());
}

void TVao::Bind() const
{
    if (ActiveHandle_ != Handle_) {
        glBindVertexArray(Handle_);
        ActiveHandle_ = Handle_;
    }
}

void TVao::Unbind()
{
    // NB(apachee): It is a waste to do that in WebGL,
    // so skip unbinding in release.
#ifndef NDEBUG
    if (ActiveHandle_ != DefaultVaoHandle) {
        glBindVertexArray(DefaultVaoHandle);
        ActiveHandle_ = DefaultVaoHandle;
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
