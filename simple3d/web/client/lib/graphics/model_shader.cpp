#include "model_shader.h"

#include <simple3d/web/client/lib/graphics/shader_sources/shader_sources.h>

#include <simple3d/web/client/lib/webgl2/shader_program.h>
#include <simple3d/web/client/lib/webgl2/uniform_id.h>

namespace NSimple3D::NGraphics {

using namespace NWebGL2;

////////////////////////////////////////////////////////////////////////////////

const ui32 IShader::DirectionalLigthCapacity = 8;

////////////////////////////////////////////////////////////////////////////////

S3D_DECLARE_REFCOUNTED_CLASS(TModelShader)

class TModelShader
	: public IShader
{
private:
	struct TPrivateTag {};

	struct TUniforms
	{
		inline static const auto ViewPos = CreateSimpleUID("view_pos");
		inline static const auto AmbientLight = CreateSimpleUID("ambient_light_uid");

#define	FOR_EACH_FIELD(XX) \
	XX(Direction, CreateSimpleUID("direction")) \
	XX(Diffuse, CreateSimpleUID("diffuse")) \
	XX(Specular, CreateSimpleUID("specular"))

		S3D_DEFINE_STRUCT_UID_FIELD_IDS(TDirectionalLightField, FOR_EACH_FIELD);

		inline static const auto DirectionalLigthCount = CreateSimpleUID("directional_light_count");
		inline static const auto DirectionalLight = CreateArrayUID<IShader::DirectionalLigthCapacity>("directional_light", S3D_CREATE_STRUCT_UID(FOR_EACH_FIELD));

#undef FOR_EACH_FIELD

		inline static const auto View = CreateSimpleUID("view");
		inline static const auto Projection = CreateSimpleUID("projection");

		static std::vector<std::string> GetUIDs()
		{
			return MergeUIDs(
				ViewPos,
				AmbientLight,
				DirectionalLigthCount,
				DirectionalLight,
				View,
				Projection);
		}
	};

#define	FOR_EACH_FIELD(XX) \
	XX(ViewPos) \
	XX(AmbientLight) \
	XX(DirectionalLigthCount) \
	XX(DirectionalLight) \
	XX(View) \
	XX(Projection)

	S3D_DEFINE_UNIFORM_DESCRIPTORS_STRUCT(TUniformDescriptors, TUniforms, FOR_EACH_FIELD);

#undef FOR_EACH_FIELD

	using TDirectionalLightField = TUniforms::TDirectionalLightField;

public:
	TModelShader(TShaderProgram shaderProgram, TPrivateTag)
		: ShaderProgram_(std::move(shaderProgram))
		, UniformDescriptors_(TUniformDescriptors::Create(ShaderProgram_))
	{ }

	static TModelShaderPtr Create()
	{
		auto shaderProgram = TShaderProgramBuilder()
			.VertexShaderSource(ModelShaderVertexGlslSource)
			.FragmentShaderSource(ModelShaderFragmentGlslSource)
			.SetUniformNames(TUniforms::GetUIDs())
			.Build()
			.ValueOrThrow();

		return std::make_shared<TModelShader>(std::move(shaderProgram), TPrivateTag{});
	}

    void SetView(const glm::mat4& view) override
	{
		SetUniform(UniformDescriptors_.View, view);
	}

    void SetProj(const glm::mat4& projection) override
	{
		SetUniform(UniformDescriptors_.Projection, projection);
	}

    void SetViewPos(const glm::vec3& pos) override
	{
		SetUniform(UniformDescriptors_.ViewPos, pos);
	}

    void SetDirectionalLights(
        const std::unordered_set<std::shared_ptr<TDirectionalLight>>& lights) override
	{
		if (lights.size() > DirectionalLigthCapacity) {
			S3D_THROW("Number of lights exceeds maximum capacity (NumberOfLights: {}, MaxCapacity: {})", lights.size(), DirectionalLigthCapacity);
		}
		SetUniform(UniformDescriptors_.DirectionalLigthCount, lights.size());

		{
			int index = 0;

			for (const auto& light : lights) {
				const auto& lightDescriptor = UniformDescriptors_.DirectionalLight[index];

				SetUniform(lightDescriptor.Get<TDirectionalLightField::Direction>(), light->direction);
				SetUniform(lightDescriptor.Get<TDirectionalLightField::Diffuse>(), light->diffuse);
				SetUniform(lightDescriptor.Get<TDirectionalLightField::Specular>(), light->specular);

				index++;
			}
		}
	}

    void SetAmbientLight(const glm::vec3& color) override
	{
		SetUniform(UniformDescriptors_.AmbientLight, color);
	}

    void Use() const override
	{

	}

private:
	template <typename T>
	void SetUniform(TUniformDescriptor descriptor, T&& value)
	{
		ShaderProgram_.SetUniform(std::move(descriptor), std::forward<T>(value));
	}

	const TShaderProgram ShaderProgram_;
	const TUniformDescriptors UniformDescriptors_;
};

////////////////////////////////////////////////////////////////////////////////

IShaderPtr CreateModelShader()
{
	return TModelShader::Create();
}

////////////////////////////////////////////////////////////////////////////////

void TVertex::BindAttributes()
{
    glVertexAttribPointer(
        0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TVertex),
		reinterpret_cast<void*>(offsetof(TVertex, Pos)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TVertex),
		reinterpret_cast<void*>(offsetof(TVertex, Normal)));
    glEnableVertexAttribArray(1);
}

////////////////////////////////////////////////////////////////////////////////

void TTexturedVertex::BindAttributes()
{
    glVertexAttribPointer(
        0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TTexturedVertex),
		reinterpret_cast<void*>(offsetof(TTexturedVertex, Pos)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TTexturedVertex),
		reinterpret_cast<void*>(offsetof(TTexturedVertex, Normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TTexturedVertex),
		reinterpret_cast<void*>(offsetof(TTexturedVertex,
		TexCoords)));
    glEnableVertexAttribArray(2);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NGraphics
