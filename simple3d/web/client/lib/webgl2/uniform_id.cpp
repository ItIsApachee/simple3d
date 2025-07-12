#include "uniform_id.h"

#include <simple3d/web/client/lib/webgl2/shader_program.h>

#include <simple3d/core/compiler.h>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

namespace NDetail {

////////////////////////////////////////////////////////////////////////////////

TSimpleUID<false>::TSimpleUID(std::string name)
    : Name_(std::move(name))
{ }

TSimpleUID<false>::TWithDescriptor TSimpleUID<false>::FillDescriptor(const TShaderProgram& shaderProgram) &&
{
    auto descriptor = shaderProgram.GetUniformDescriptor(Name_);
    return TWithDescriptor(std::move(Name_), descriptor);
}

std::vector<std::string> TSimpleUID<false>::GetUIDs() const
{
    return {Name_};
}

TSimpleUID<false> TSimpleUID<false>::WithPrefix(const std::string& prefix) const
{
    return TSimpleUID(prefix + Name_);
}

////////////////////////////////////////////////////////////////////////////////

TSimpleUID<true>::TSimpleUID(std::string name, TUniformDescriptor descriptor)
    : Name_(std::move(name))
    , Descriptor_(descriptor)
{ }

TSimpleUID<true>::TWithDescriptor TSimpleUID<true>::FillDescriptor(const TShaderProgram& shaderProgram) &&
{
    auto descriptor = shaderProgram.GetUniformDescriptor(Name_);
    return TWithDescriptor(std::move(Name_), descriptor);
}

std::vector<std::string> TSimpleUID<true>::GetUIDs() const
{
    return {Name_};
}

TSimpleUID<true> TSimpleUID<true>::WithPrefix(const std::string& prefix) const
{
    return TSimpleUID(prefix + Name_, std::move(Descriptor_));
}

TUniformDescriptor TSimpleUID<true>::GetDescriptor() const
{
    return Descriptor_;
}

TSimpleUID<true>::operator TUniformDescriptor () const
{
    return GetDescriptor();
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NDetail

TSimpleUID CreateSimpleUID(std::string name)
{
    return TSimpleUID(std::move(name));
}

// FIXME(apachee): Add tests.

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
