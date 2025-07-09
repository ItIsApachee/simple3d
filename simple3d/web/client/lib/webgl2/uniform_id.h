#pragma once

#include "shader_program.h"

#include <simple3d/core/types.h>

#include <string>
#include <tuple>

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

namespace NDetail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
concept CUniformID =
    // Basic requirements for UIDs.
    requires (const T& v, const std::string& prefix) {
        typename T::TWithDescriptor;
        { v.GetUIDs() } -> std::same_as<std::vector<std::string>>;
    }
    && requires (T&& v, const TShaderProgram& shaderProgram) {
        { std::move(v).FillDescriptor(shaderProgram) } -> std::same_as<typename T::TWithDescriptor>;
    }
    // Ensure all terminal UIDs have descriptor getter.
    && (!T::IsTerminal || requires (const typename T::TWithDescriptor& v) {
        { v.GetDescriptor() } -> std::same_as<TUniformDescriptor>;
    })
    // Ensure we can add prefix to all UIDs.
    && requires (const T& v, const std::string& prefix) {
        { v.WithPrefix(prefix) } -> std::same_as<T>;
    };

template <bool WithDescriptor = false>
class TSimpleUID
{ };

template <>
class TSimpleUID<false>
{
public:
    static constexpr bool IsTerminal = true;
    using TWithDescriptor = TSimpleUID<true>;
    using TWithoutDescriptor = TSimpleUID<false>;

    TSimpleUID() = default;

    explicit TSimpleUID(std::string name);

    TWithDescriptor FillDescriptor(const TShaderProgram& shaderProgram) &&;
    std::vector<std::string> GetUIDs() const;
    TSimpleUID WithPrefix(const std::string& prefix) const;

private:
    std::string Name_;
};

template <>
class TSimpleUID<true>
{
public:
    static constexpr bool IsTerminal = true;
    using TWithDescriptor = TSimpleUID<true>;
    using TWithoutDescriptor = TSimpleUID<false>;

    TSimpleUID() = default;

    TSimpleUID(std::string name, TUniformDescriptor descriptor);

    TWithDescriptor FillDescriptor(const TShaderProgram& shaderProgram) &&;
    std::vector<std::string> GetUIDs() const;
    TSimpleUID WithPrefix(const std::string& prefix) const;

    TUniformDescriptor GetDescriptor() const;

private:
    std::string Name_;
    TUniformDescriptor Descriptor_;
};

static_assert(CUniformID<TSimpleUID<>>);
static_assert(CUniformID<TSimpleUID<true>>);

template <ui32 Size, CUniformID TItem>
class TArrayUID
{
public:
    static constexpr bool IsTerminal = false;
    using TWithDescriptor = TArrayUID<Size, typename TItem::TWithDescriptor>;
    using TWithoutDescriptor = TArrayUID<Size, typename TItem::TWithoutDescriptor>;

    friend TWithoutDescriptor;

    TArrayUID() = default;

    TArrayUID(std::string name, TItem item);

    TWithDescriptor FillDescriptor(const TShaderProgram& shaderProgram) &&;
    std::vector<std::string> GetUIDs() const;
    TArrayUID WithPrefix(const std::string& prefix) const;

    const TItem& operator[](ui32 index) const;

private:
    explicit TArrayUID(std::array<TItem, Size> items);

    std::array<TItem, Size> Items_;
};

static_assert(CUniformID<TArrayUID<3, TSimpleUID<>>>);
static_assert(CUniformID<TArrayUID<3, TSimpleUID<true>>>);

template <CUniformID... TFields>
class TStructUID
{
public:
    struct TNoNameTag
    { };

    static constexpr bool IsTerminal = false;
    using TWithDescriptor = TStructUID<typename TFields::TWithDescriptor...>;
    using TWithoutDescriptor = TStructUID<typename TFields::TWithoutDescriptor...>;

    friend TWithoutDescriptor;

    TStructUID() = default;

    explicit TStructUID(TNoNameTag, TFields... fields);
    explicit TStructUID(std::string name, TFields... fields);

    TWithDescriptor FillDescriptor(const TShaderProgram& shaderProgram) &&;
    std::vector<std::string> GetUIDs() const;
    TStructUID WithPrefix(const std::string& prefix) const;

    template <std::size_t Index>
    auto Get() const -> const std::tuple_element_t<Index, const std::tuple<TFields...>>&;

private:
    struct TPrivateTag
    { };

    struct TPrefixTag
    { };

    TStructUID(TPrivateTag, std::tuple<TFields...> fields);
    TStructUID(TPrefixTag, const std::string& prefix, TFields... fields);

    std::tuple<TFields...> Fields_;
};

static_assert(CUniformID<
    TStructUID<
        TSimpleUID<>,
        TArrayUID<
            3, TSimpleUID<>
        >,
        TSimpleUID<>
    >
>);

static_assert(CUniformID<
    TStructUID<
        TSimpleUID<>,
        TArrayUID<
            3, TSimpleUID<>
        >,
        TSimpleUID<>
    >::TWithDescriptor
>);

// TODO(apachee): Add macro for cleaner code with structs.
/*

#define FOR_EACH_FIELD(XX) \
    XX(PascalCase, type), \
    XX(PascalCase, type), \
    XX(PascalCase, type), \
    XX(PascalCase, type) \

S3D_DEFINE_STRUCT_UID(FieldEnumName, FOR_EACH_FIELD);

S3D_CREATE_STRUCT_UID(FOR_EACH_FIELD);
// or
S3D_CREATE_STRUCT_UID_WITH_NAME(StructName, FOR_EACH_FIELD);

#undef FOR_EACH_FIELD

////////////////////////////////////////////////////////////////////////////////

#define _S3D_DEFINE_STRUCT_UID_ENUM_ENTRY(PascalCase, ...) \
    PascalCase

#define S3D_DEFINE_STRUCT_UID(FIELD_ENUM_NAME, FOR_EACH_FIELD) \
    enum FIELD_ENUM_NAME \
    { \
        FOR_EACH_FIELD(_S3D_DEFINE_STRUCT_UID_ENUM_ENTRY)
    }; \
    static_assert(true)

#define _S3D_CREATE_STRUCT_UID_FIELD(PASCAL_CASE, TYPE) \
    TYPE

#define S3D_CREATE_STRUCT_UID(FOR_EACH_FIELD) \
    CreateStructUID( \
    FOR_EACH_FIELD(_S3D_CREATE_STRUCT_UID_FIELD) \
    )

*/

////////////////////////////////////////////////////////////////////////////////

}  // namespace NDetail

using TSimpleUID = NDetail::TSimpleUID<>;

using NDetail::TArrayUID;
using NDetail::TStructUID;
using NDetail::CUniformID;

TSimpleUID CreateSimpleUID(std::string name);

template <ui32 Size, CUniformID TItem>
TArrayUID<Size, TItem> CreateArrayUID(std::string name, TItem item);

template <CUniformID... TFields>
TStructUID<TFields...> CreateStructUID(TFields... fields);

template <CUniformID... TFields>
TStructUID<TFields...> CreateStructUIDWithName(std::string name, TFields... fields);

const TSimpleUID EmptySimpleUID = CreateSimpleUID("");

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2

#define S3D_LIB_WEBGL2_UNIFORM_ID_INL_H
#include "uniform_id-inl.h"
#undef S3D_LIB_WEBGL2_UNIFORM_ID_INL_H