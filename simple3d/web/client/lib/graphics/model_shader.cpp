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

    bool Use() const override
	{
		return ShaderProgram_.Use();
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

void TTriangle::BindAttributes()
{
	static constexpr GLuint ModelIndex = 0;
	static constexpr int ModelRowCount = 4;
	static constexpr int ModelRowSize = sizeof(glm::vec4);
	static constexpr int ModelRowComponentCount = 4;

	static constexpr GLuint PosIndex = 4;
	static constexpr int PosComonentCount = 3;
	static constexpr GLuint DiffuseIndex = 7;
	static constexpr int DiffuseComonentCount = 3;
	static constexpr GLuint SpecularIndex = 10;
	static constexpr int SpecularComonentCount = 3;
	static constexpr int VertexCount = 3;
	static constexpr GLuint ShininessIndex = 13;
	static constexpr int ShininessComponentCount = 3;
	static constexpr int VertexSize = sizeof(TVertex);

	static constexpr GLuint NormalIndex = 14;
	static constexpr int NormalComponentCount = 3;

	static constexpr auto Stride = sizeof(TTriangle);

	for (int i = 0; i < ModelRowCount; i++) {
		glVertexAttribPointer(
			ModelIndex + i,
			ModelRowComponentCount,
			GL_FLOAT,
			GL_FALSE,
			Stride,
			reinterpret_cast<void*>(
				offsetof(TTriangle, Model) + ModelRowSize * i));
		glEnableVertexAttribArray(ModelIndex + i);
		glVertexAttribDivisor(ModelIndex + i, 1);
	}

	for (int i = 0; i < VertexCount; i++) {
		// XXX(apachee): Is behavior here well-defined regardless of the alignment?

		glVertexAttribPointer(
			PosIndex + i,
			PosComonentCount,
			GL_FLOAT,
			GL_FALSE,
			Stride,
			reinterpret_cast<void*>(
				offsetof(TTriangle, Vertices) + VertexSize * i + offsetof(TVertex, Pos)));
		glEnableVertexAttribArray(PosIndex + i);
		glVertexAttribDivisor(PosIndex + i, 1);

		glVertexAttribPointer(
			DiffuseIndex + i,
			DiffuseComonentCount,
			GL_FLOAT,
			GL_FALSE,
			Stride,
			reinterpret_cast<void*>(
				offsetof(TTriangle, Vertices) + VertexSize * i + offsetof(TVertex, Diffuse)));
		glEnableVertexAttribArray(DiffuseIndex + i);
		glVertexAttribDivisor(DiffuseIndex + i, 1);

		glVertexAttribPointer(
			SpecularIndex + i,
			SpecularComonentCount,
			GL_FLOAT,
			GL_FALSE,
			Stride,
			reinterpret_cast<void*>(
				offsetof(TTriangle, Vertices) + VertexSize * i + offsetof(TVertex, Specular)));
		glEnableVertexAttribArray(SpecularIndex + i);
		glVertexAttribDivisor(SpecularIndex + i, 1);
	}

	glVertexAttribPointer(
		ShininessIndex,
		ShininessComponentCount,
		GL_FLOAT,
		GL_FALSE,
		Stride,
		reinterpret_cast<void*>(offsetof(TTriangle, Shininess)));
	glEnableVertexAttribArray(ShininessIndex);
	glVertexAttribDivisor(ShininessIndex, 1);

	glVertexAttribPointer(
		NormalIndex,
		NormalComponentCount,
		GL_FLOAT,
		GL_FALSE,
		Stride,
		reinterpret_cast<void*>(offsetof(TTriangle, Normal)));
	glEnableVertexAttribArray(NormalIndex);
	glVertexAttribDivisor(NormalIndex, 1);
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
