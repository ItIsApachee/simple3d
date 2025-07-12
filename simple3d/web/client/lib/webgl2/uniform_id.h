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

    operator TUniformDescriptor () const;

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

struct TDummyArg
{ };

//! Helper for macro impl.
template <CUniformID... TFields>
TStructUID<TFields...> CreateStructUID(TDummyArg, TFields... fields);

////////////////////////////////////////////////////////////////////////////////

}  // namespace NDetail

////////////////////////////////////////////////////////////////////////////////

using TSimpleUID = NDetail::TSimpleUID<>;

using NDetail::TArrayUID;
using NDetail::TStructUID;
using NDetail::CUniformID;

////////////////////////////////////////////////////////////////////////////////

TSimpleUID CreateSimpleUID(std::string name);

template <ui32 Size, CUniformID TItem>
TArrayUID<Size, TItem> CreateArrayUID(std::string name, TItem item);

template <CUniformID... TFields>
TStructUID<TFields...> CreateStructUID(TFields... fields);

template <CUniformID... TFields>
TStructUID<TFields...> CreateStructUIDWithName(std::string name, TFields... fields);

////////////////////////////////////////////////////////////////////////////////

const TSimpleUID EmptySimpleUID = CreateSimpleUID("");

////////////////////////////////////////////////////////////////////////////////

template <CUniformID... TUniformIDs>
std::vector<std::string> MergeUIDs(const TUniformIDs&... uniformIDs);

////////////////////////////////////////////////////////////////////////////////

#define _S3D_DEFINE_STRUCT_UID_FIELD_IDS_ENUM_ENTRY(PASCAL_CASE_NAME, ...) \
    PASCAL_CASE_NAME, \

#define _S3D_DEFINE_STRUCT_UID_FIELD_IDS_STRUCT_FIELD(PASCAL_CASE_NAME, ...) \
    static constexpr auto PASCAL_CASE_NAME = static_cast<std::size_t>(_EEnum :: PASCAL_CASE_NAME);

#define S3D_DEFINE_STRUCT_UID_FIELD_IDS(STRUCT_UID_FIELD_IDS_NAME, FOR_EACH_FIELD) \
    enum struct _ ## STRUCT_UID_FIELD_IDS_NAME ## Internal : std::size_t \
    { \
        FOR_EACH_FIELD(_S3D_DEFINE_STRUCT_UID_FIELD_IDS_ENUM_ENTRY)\
    }; \
    \
    struct STRUCT_UID_FIELD_IDS_NAME \
    { \
        using _EEnum = _ ## STRUCT_UID_FIELD_IDS_NAME ## Internal; \
        \
        FOR_EACH_FIELD(_S3D_DEFINE_STRUCT_UID_FIELD_IDS_STRUCT_FIELD)\
    }; \
    static_assert(true)

#define _S3D_CREATE_STRUCT_UID_FIELD(_, TYPE) \
    , TYPE

#define S3D_CREATE_STRUCT_UID(FOR_EACH_FIELD) \
    ::NSimple3D::NWebGL2::NDetail::CreateStructUID( \
        ::NSimple3D::NWebGL2::NDetail::TDummyArg{} \
        FOR_EACH_FIELD(_S3D_CREATE_STRUCT_UID_FIELD))

#define S3D_CREATE_STRUCT_UID_WITH_NAME(name, FOR_EACH_FIELD) \
    ::NSimple3D::NWebGL2::NDetail::CreateStructUID( \
        name \
        FOR_EACH_FIELD(_S3D_CREATE_STRUCT_UID_FIELD))

////////////////////////////////////////////////////////////////////////////////

#define _S3D_DEFINE_UNIFORM_DESCRIPTORS_STRUCT_FIELD(FIELD_NAME) \
    decltype(TBaseStruct :: FIELD_NAME)::TWithDescriptor FIELD_NAME;

#define _S3D_DEFINE_UNIFORM_DESCRIPTORS_STRUCT_FIELD_INITIALIZER(FIELD_NAME) \
    .FIELD_NAME = std::remove_const_t<decltype(TBaseStruct :: FIELD_NAME)>(TBaseStruct :: FIELD_NAME).FillDescriptor(shaderProgram),

#define S3D_DEFINE_UNIFORM_DESCRIPTORS_STRUCT(STRUCT_NAME, BASE_STRUCT_NAME, FOR_EACH_FIELD) \
    struct STRUCT_NAME \
    { \
        using TBaseStruct = BASE_STRUCT_NAME; \
        \
        static STRUCT_NAME Create(const TShaderProgram& shaderProgram) \
        { \
            return STRUCT_NAME{ \
                FOR_EACH_FIELD(_S3D_DEFINE_UNIFORM_DESCRIPTORS_STRUCT_FIELD_INITIALIZER) \
            }; \
        } \
        \
        FOR_EACH_FIELD(_S3D_DEFINE_UNIFORM_DESCRIPTORS_STRUCT_FIELD) \
    }; \
    static_assert(true)

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2

#define S3D_LIB_WEBGL2_UNIFORM_ID_INL_H
#include "uniform_id-inl.h"
#undef S3D_LIB_WEBGL2_UNIFORM_ID_INL_H
