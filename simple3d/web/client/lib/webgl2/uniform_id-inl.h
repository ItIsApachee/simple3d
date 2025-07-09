#include <simple3d/core/types.h>
#include <utility>
#ifndef S3D_LIB_WEBGL2_UNIFORM_ID_INL_H
#error This file should not be included directly, use .h version
#include "uniform_id.h"
#endif

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

namespace NDetail {

////////////////////////////////////////////////////////////////////////////////

template <ui32 Size, CUniformID TItem>
TArrayUID<Size, TItem>::TArrayUID(std::string name, TItem item)
{
    for (ui32 i = 0; i < Size; i++) {
        auto prefix = std::format("{}[{}]", name, i);
        Items_[i] = item.WithPrefix(prefix);
    }
}

template <ui32 Size, CUniformID TItem>
TArrayUID<Size, TItem>::TWithDescriptor TArrayUID<Size, TItem>::FillDescriptor(const TShaderProgram& shaderProgram) &&
{
    std::array<typename TItem::TWithDescriptor, Size> itemsWithDescriptor{};
    for (ui32 i = 0; i < Size; i++) {
        itemsWithDescriptor[i] = std::move(Items_[i]).FillDescriptor(shaderProgram);
    }
    return TWithDescriptor(std::move(itemsWithDescriptor));
}

template <ui32 Size, CUniformID TItem>
std::vector<std::string> TArrayUID<Size, TItem>::GetUIDs() const
{
    if (Size == 0) {
        return {};
    }

    std::vector<std::string> result;
    for (ui32 i = 0; i < Size; i++) {
        auto itemIDs = Items_[i].GetUIDs();

        // Small-to-large optimization
        if (itemIDs.size() > result.size()) {
            std::swap(itemIDs, result);
        }

        // XXX(apachee): Maybe use move iterator.
        for (auto& id : itemIDs) {
            result.push_back(std::move(id));
        }
    }

    return result;
}

template <ui32 Size, CUniformID TItem>
TArrayUID<Size, TItem> TArrayUID<Size, TItem>::WithPrefix(const std::string& prefix) const
{
    std::array<TItem, Size> itemsWithPrefix{};
    for (ui32 i = 0; i < Size; i++) {
        itemsWithPrefix[i] = Items_[i].WithPrefix(prefix);
    }
    return TArrayUID(std::move(itemsWithPrefix));
}

template <ui32 Size, CUniformID TItem>
const TItem& TArrayUID<Size, TItem>::operator[](ui32 index) const
{
    if (index >= Size) {
        S3D_THROW("Index is out of bounds (Index: {}, Size: {})", index, Size);
    }
    return Items_[index];
}

template <ui32 Size, CUniformID TItem>
TArrayUID<Size, TItem>::TArrayUID(std::array<TItem, Size> items)
    : Items_(std::move(items))
{ }

////////////////////////////////////////////////////////////////////////////////

template <CUniformID... TFields>
TStructUID<TFields...>::TStructUID(TNoNameTag, TFields... fields)
    : TStructUID(TPrefixTag{}, ".", std::move(fields)...)
{ }

template <CUniformID... TFields>
TStructUID<TFields...>::TStructUID(std::string name, TFields... fields)
    : TStructUID(TPrefixTag{}, std::format("{}.", name), std::move(fields)...)
{ }

// XXX(apachee): Make code cleaner here?

template <CUniformID... TFields, ui32... Indexes>
auto StructUIDFillDescriptorImpl(std::tuple<TFields...> fields, const TShaderProgram& shaderProgram, std::integer_sequence<ui32, Indexes...>)
{
    return std::tuple(std::move(std::get<Indexes>(fields)).FillDescriptor(shaderProgram)...);
}

template <CUniformID... TFields>
TStructUID<TFields...>::TWithDescriptor TStructUID<TFields...>::FillDescriptor(const TShaderProgram& shaderProgram) &&
{
    return TWithDescriptor(typename TWithDescriptor::TPrivateTag{}, StructUIDFillDescriptorImpl(std::move(Fields_), shaderProgram, std::make_integer_sequence<ui32, sizeof...(TFields)>()));
}

// XXX(apachee): Make code cleaner here?

template <CUniformID... TFields, ui32... Indexes>
auto StructUIDGetUIDsImpl(std::tuple<TFields...> fields, std::integer_sequence<ui32, Indexes...>)
{
    std::vector<std::string> result;
    // XXX(apachee): Is it guaranteed that this processes fields in the same order they appear?
    (
        [&] {
            auto fieldUIDs = std::get<Indexes>(fields).GetUIDs();

            // Small-to-large optimization
            if (fieldUIDs.size() > result.size()) {
                std::swap(fieldUIDs, result);
            }

            // XXX(apachee): Maybe use move iterator.
            for (auto& id : fieldUIDs) {
                result.push_back(std::move(id));
            }
        } (),
        ...
    );
    return result;
}

template <CUniformID... TFields>
std::vector<std::string> TStructUID<TFields...>::GetUIDs() const
{
    return StructUIDGetUIDsImpl(Fields_, std::make_integer_sequence<ui32, sizeof...(TFields)>());
}

// XXX(apachee): Make code cleaner here?

template <CUniformID... TFields, ui32... Indexes>
auto StructUIDWithPrefixImpl(std::tuple<TFields...> fields, const std::string& prefix, std::integer_sequence<ui32, Indexes...>)
{
    return std::tuple(std::get<Indexes>(fields).WithPrefix(prefix)...);
}

template <CUniformID... TFields>
TStructUID<TFields...> TStructUID<TFields...>::WithPrefix(const std::string& prefix) const
{
    return TStructUID(TPrivateTag{}, StructUIDWithPrefixImpl(Fields_, prefix, std::make_integer_sequence<ui32, sizeof...(TFields)>()));
}

template <CUniformID... TFields>
template <std::size_t Index>
auto TStructUID<TFields...>::Get() const -> const std::tuple_element_t<Index, const std::tuple<TFields...>>&
{
    return std::get<Index>(Fields_);
}

template <CUniformID... TFields>
TStructUID<TFields...>::TStructUID(TPrivateTag, std::tuple<TFields...> fields)
    : Fields_(std::move(fields))
{ }

template <CUniformID... TFields>
TStructUID<TFields...>::TStructUID(TPrefixTag, const std::string& prefix, TFields... fields)
    : Fields_(fields.WithPrefix(prefix)...)
{ }

////////////////////////////////////////////////////////////////////////////////

template <CUniformID... TFields>
TStructUID<TFields...> CreateStructUID(TDummyArg, TFields... fields)
{
    return ::NSimple3D::NWebGL2::CreateStructUID(std::move(fields)...);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NDetail

////////////////////////////////////////////////////////////////////////////////

template <ui32 Size, CUniformID TItem>
TArrayUID<Size, TItem> CreateArrayUID(std::string name, TItem item)
{
    return TArrayUID<Size, TItem>(std::move(name), std::move(item));
}

template <CUniformID... TFields>
TStructUID<TFields...> CreateStructUID(TFields... fields)
{
    return TStructUID<TFields...>(
        typename TStructUID<TFields...>::TNoNameTag{},
        std::move(fields)...);
}

template <CUniformID... TFields>
TStructUID<TFields...> CreateStructUIDWithName(std::string name, TFields... fields)
{
    return TStructUID<TFields...>(
        std::move(name),
        std::move(fields)...);
}

////////////////////////////////////////////////////////////////////////////////

template <CUniformID... TUniformIDs>
std::vector<std::string> MergeUIDs(const TUniformIDs&... uniformIDs)
{
    std::vector<std::string> result;

    (
        [&] {
            for (auto& id : uniformIDs.GetUIDs()) {
                result.push_back(std::move(id));
            }
        } (),
        ...
    );

    return result;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
