#include <simple3d/graphics/models/model.h>

#include <cstdint>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

// FIXME: remove
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/gles2.h>
#include <simple3d/graphics/internal/element_buffer_object.h>
#include <simple3d/graphics/internal/gles_shader.h>
#include <simple3d/graphics/internal/vertex.h>
#include <simple3d/graphics/internal/vertex_array_object.h>
#include <simple3d/graphics/internal/vertex_buffer_object.h>
#include <stb/stb_image.h>

#include <assimp/Importer.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <iostream>

namespace Simple3D {

// TODO(apachee): add Internal::Texture class
constexpr char kDiffuseTexture[] = "texture_diffuse";
constexpr char kSpecularTexture[] = "texture_specular";

struct Texture {
 public:
  GLuint id{0};
  std::string type{};
};

class TextureLoader {
 public:
  TextureLoader(const aiScene* scene,
                const std::filesystem::path& textures_path)
      : scene{scene}, textures_path{textures_path} {}
  TextureLoader(const TextureLoader&) = default;
  TextureLoader& operator=(const TextureLoader&) = default;
  ~TextureLoader() = default;

  std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type,
                                    const std::string& type_name) {
    std::vector<Texture> textures;
    for (std::int64_t i = 0; i < mat->GetTextureCount(type); i++) {
      aiString ai_str;
      mat->GetTexture(type, i, &ai_str);

      // check if already loaded
      std::string str{ai_str.C_Str()};
      if (auto it = loaded.find(str); it != loaded.end()) {
        textures.push_back(it->second);
        continue;
      }

      // create texture
      Texture texture;

      const aiTexture* ai_texture = scene->GetEmbeddedTexture(ai_str.C_Str());
      // TODO(apachee): add support for embedded textures
      assert(ai_texture == nullptr);

      int width, height, channels;
      std::string filename = (textures_path / ai_str.C_Str()).string();
      unsigned char* data =
          stbi_load(filename.c_str(), &width, &height, &channels, 0);

      GLenum format{};
      if (channels == 1) {
        format = GL_RED;
      } else if (channels == 3) {
        format = GL_RGB;
      } else if (channels == 4) {
        format = GL_RGBA;
      } else {
        assert(false);
      }

      glGenTextures(1, &texture.id);

      glBindTexture(GL_TEXTURE_2D, texture.id);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);

      texture.type = type_name;

      textures.push_back(texture);
      loaded.emplace(str, texture);
    }

    return textures;
  }

 private:
  const aiScene* scene{nullptr};
  std::filesystem::path textures_path{};

  std::unordered_map<std::string, Texture> loaded{};
};

class Mesh {
 public:
  static Mesh LoadMesh(aiMesh* mesh, const aiScene* scene,
                       TextureLoader* texture_loader) {
    std::vector<Internal::TexturedVertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (std::int64_t i = 0; i < mesh->mNumVertices; i++) {
      Internal::TexturedVertex vertex{};

      // position
      vertex.position.x = mesh->mVertices[i].x;
      vertex.position.y = mesh->mVertices[i].y;
      vertex.position.z = mesh->mVertices[i].z;

      // normal
      vertex.normal.x = mesh->mNormals[i].x;
      vertex.normal.y = mesh->mNormals[i].y;
      vertex.normal.z = mesh->mNormals[i].z;

      // tex coords
      if (mesh->mTextureCoords[0]) {
        vertex.texture_coords.x = mesh->mTextureCoords[0][i].x;
        vertex.texture_coords.y = mesh->mTextureCoords[0][i].y;
      } else {
        vertex.texture_coords = glm::vec2(0.0f);
      }

      vertices.push_back(std::move(vertex));
    }

    for (std::int64_t i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      assert(face.mNumIndices == 3);
      for (std::int64_t j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }

    if (mesh->mMaterialIndex >= 0) {
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

      for (auto& diffuse_texture : texture_loader->LoadTextures(
               material, aiTextureType_DIFFUSE, kDiffuseTexture)) {
        textures.push_back(std::move(diffuse_texture));
      }

      for (auto& specular_texture : texture_loader->LoadTextures(
               material, aiTextureType_SPECULAR, kSpecularTexture)) {
        textures.push_back(std::move(specular_texture));
      }
    }

    return Mesh(vertices, indices, textures);
  }

  Mesh() = default;
  Mesh(const std::vector<Internal::TexturedVertex>& vertices,
       const std::vector<GLuint>& indices, const std::vector<Texture>& textures)
      : textures{textures}, indices_size{(std::int64_t)indices.size()} {
    vertex_array_.Bind();

    vertex_buffer_ = Internal::VertexBufferObject(
        sizeof(Internal::TexturedVertex) * vertices.size(),
        reinterpret_cast<const std::byte*>(vertices.data()));
    vertex_buffer_.Bind();
    Internal::TexturedVertex::BindAttributes();

    indices_data_ =
        Internal::ElementBufferObjectBuilder()
            .Data(sizeof(GLuint) * indices.size(),
                  reinterpret_cast<const std::byte*>(indices.data()))
            .Build(vertex_array_);
    vertex_array_.BindEbo(indices_data_);

    vertex_array_.Unbind();
  }

  Mesh(const Mesh&) = delete;
  Mesh(Mesh&&) = default;
  Mesh& operator=(const Mesh&) = delete;
  Mesh& operator=(Mesh&&) = default;
  ~Mesh() = default;

  void Draw(const Internal::GlesShader& shader) const {
    shader.Use();

    // naming scheme:
    // material.texture_diffuse_{i}
    // material.texture_specular_{i}
    // where i is index
    std::string texture_prefix = "material";

    std::int64_t diffuse_textures_cnt = 0;
    std::int64_t specular_textures_cnt = 0;

    for (std::int64_t i = 0; i < textures.size(); i++) {
      glActiveTexture((GLenum)(GL_TEXTURE0 + i));

      std::string num{};
      const std::string& name = textures[i].type;
      if (name == kDiffuseTexture) {
        num = std::to_string(diffuse_textures_cnt++);
      } else if (name == kSpecularTexture) {
        num = std::to_string(specular_textures_cnt++);
      } else {
        assert(false);  // shouldn't be happening
      }

      shader.SetUniform1i(texture_prefix + name + num, i);
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);  // at least 1 texture unit

    vertex_array_.Bind();
    glDrawElements(GL_TRIANGLES, (GLsizei)indices_size, GL_UNSIGNED_INT, 0);
  }

 private:
  std::int64_t indices_size{0};

  Internal::VertexBufferObject vertex_buffer_{};
  Internal::VertexArrayObject vertex_array_{};
  Internal::ElementBufferObject indices_data_{};

  std::vector<Texture> textures;
};

std::shared_ptr<Model> Model::Load(
    const std::filesystem::path& model_path,
    std::optional<std::filesystem::path> textures_path_opt) {
  Assimp::Importer importer{};
  const aiScene* scene = importer.ReadFile(
      model_path.string(),
      aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

  if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
      !scene->mRootNode) {
    // TODO(apachee): add handling
    assert(false);
  }

  std::filesystem::path textures_path = textures_path_opt.value_or(
      std::filesystem::path(model_path).remove_filename());

  std::shared_ptr<Model> model{new Model{}};
  TextureLoader texture_loader(scene, textures_path);

  std::queue<aiNode*> bfs_queue;
  bfs_queue.push(scene->mRootNode);

  while (bfs_queue.size()) {
    aiNode* node = bfs_queue.front();
    bfs_queue.pop();

    for (std::int64_t i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      model->meshes.push_back(Mesh::LoadMesh(mesh, scene, texture_loader));
    }

    for (std::int64_t i = 0; i < node->mNumChildren; i++) {
      bfs_queue.push(node->mChildren[i]);
    }
  }

  return model;
}

void Model::Draw(const Internal::GlesShader& shader) const {
  for (const auto& mesh : meshes) {
    mesh.Draw(shader);
  }
}

ModelInstance* ModelRenderer::Create(const std::shared_ptr<Model>& model,
                                     const glm::vec3& pos) {
  auto model_group_it = models_.find(model.get());
  if (model_group_it == models_.end()) {
    model_group_it = models_.emplace(model.get(), ModelGroup{model}).first;
  }

  ModelInstance* model_instance_ptr{nullptr};
  {
    std::unique_ptr<ModelInstance> model_instance{new ModelInstance{pos}};
    model_instance_ptr = model_instance.get();
    model_group_it->second.instances.emplace(model_instance_ptr,
                                             std::move(model_instance));
  }

  reverse_lookup_[model_instance_ptr] = model.get();

  return model_instance_ptr;
}

void ModelRenderer::Draw(IShader& shader_interface) {
  auto& shader = dynamic_cast<TexturedModelShader&>(shader_interface);
  for (auto& [model_ptr_, model_group] : models_) {
    auto& [model, instances] = model_group;
    for (auto& [instance_ptr_, instance] : instances) {
      // setup instance vertex data

      glm::mat4 transform(1.0f);
      transform = glm::translate(transform, instance->pos);
      shader.shader().SetUniformMat4fv("model", transform);
      model->Draw(shader.shader());
    }
  }
}

void ModelRenderer::NotifyUpdated(void*) {
  // currently there is nothing it can do
}

void ModelRenderer::Destroy(void* model_instance_void) {
  ModelInstance* model_instance_ptr =
      reinterpret_cast<ModelInstance*>(model_instance_void);

  auto model_it = reverse_lookup_.find(model_instance_ptr);
  assert(model_it != reverse_lookup_.end());

  auto model_group_it = models_.find(model_it->second);
  assert(model_group_it != models_.end());

  auto& instances = model_group_it->second.instances;
  auto model_instance_it = instances.find(model_instance_ptr);
  assert(model_instance_it != instances.end());

  instances.erase(model_instance_it);
}

}  // namespace Simple3D
