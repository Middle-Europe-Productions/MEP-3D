#include <MEP-3D/model.hpp>
#include <MEP-3D/shader.hpp>
#include <MEP-3D/thread_pool.hpp>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace {
constexpr bool kUseReversedNormals = true;

void LoadMesh(aiMesh* mesh,
              const aiScene* scene,
              std::vector<MeshBaseFactoryPtr>& master_mesh_factory_,
              std::vector<unsigned int>& mesh_to_texture) {
  std::vector<GLfloat> vertices;
  std::vector<unsigned int> indices;
  for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
    vertices.insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y,
                                     mesh->mVertices[i].z});
    if (mesh->mTextureCoords[0]) {
      vertices.insert(vertices.end(), {mesh->mTextureCoords[0][i].x,
                                       mesh->mTextureCoords[0][i].y});
    } else {
      vertices.insert(vertices.end(), {0.0f, 0.0f});
    }
    if (kUseReversedNormals) {
      vertices.insert(
          vertices.end(),
          {-mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z});
    } else {
      vertices.insert(vertices.end(), {mesh->mNormals[i].x, mesh->mNormals[i].y,
                                       mesh->mNormals[i].z});
    }
  }
  for (std::size_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (std::size_t j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }
  master_mesh_factory_.emplace_back(
      std::make_unique<MeshBaseFactory>(move(vertices), std::move(indices)));
  mesh_to_texture.push_back(mesh->mMaterialIndex);
}
void LoadNode(aiNode* node,
              const aiScene* scene,
              std::vector<MeshBaseFactoryPtr>& master_mesh_factory_,
              std::vector<unsigned int>& mesh_to_texture) {
  for (std::size_t i = 0; i < node->mNumMeshes; i++) {
    LoadMesh(scene->mMeshes[node->mMeshes[i]], scene, master_mesh_factory_,
             mesh_to_texture);
  }
  for (std::size_t i = 0; i < node->mNumChildren; i++) {
    LoadNode(node->mChildren[i], scene, master_mesh_factory_, mesh_to_texture);
  }
}
void LoadTextures(const aiScene* scene,
                  std::vector<TexturePtr>& textures_container) {
  textures_container.resize(scene->mNumMaterials);
  for (std::size_t i = 0; i < scene->mNumMaterials; i++) {
    aiMaterial* material = scene->mMaterials[i];
    if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
      aiString texture_path;
      std::string filename = "";
      if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) ==
          AI_SUCCESS) {
        int id_backslash = std::string(texture_path.data).rfind("\\");
        int id_slash = std::string(texture_path.data).rfind("/");
        if (id_slash != -1) {
          filename = std::string(texture_path.data).substr(id_slash + 1);
        } else if (id_backslash != -1) {
          filename = std::string(texture_path.data).substr(id_backslash + 1);
        } else {
          filename = texture_path.data;
        }
        Image temp_image;
        if (!temp_image.LoadFromFile(kDefaultTexturePath + filename)) {
          continue;
        }
        textures_container[i] = std::make_unique<Texture>();
        textures_container[i]->Create(temp_image);
      }
    }
  }
}

}  // namespace

Model::Model() {}

void Model::Load(const std::string& file_path) {
  UpdateStatus(Status::Loading);
  ThreadPool::PostTaskWithCallback(
      Executors::Resource,
      std::make_unique<TaskWithCallback>(
          [this, file_path]() {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(
                file_path, aiProcess_Triangulate | aiProcess_FlipUVs |
                               aiProcess_GenSmoothNormals |
                               aiProcess_JoinIdenticalVertices);
            if (!scene) {
              LOG(ERROR) << "Could not load the model, path: " << file_path
                         << ", message: " << importer.GetErrorString();
              return;
            }
            LoadNode(scene->mRootNode, scene, this->master_mesh_factory_,
                     this->mesh_to_texture_);
            LoadTextures(scene, this->textures_container_);
          },
          [this]() { this->UpdateStatus(Status::Uninitialized); }));
}

void Model::Init() {
  assert(GetStatus() == Status::Uninitialized);
  for (auto& mesh_factory_ele : master_mesh_factory_) {
    mesh_container_.emplace_back(std::move(mesh_factory_ele->Create()));
  }
  master_mesh_factory_.clear();
}

void Model::Draw(RenderTarget& render_target) {
  if (GetStatus() != Status::Avalible) {
    if (GetStatus() == Status::Uninitialized) {
      Init();
    } else {
      return;
    }
  }
  if (!Get<Shader>()) {
    return;
  }
  Shader* shader = Get<Shader>();
  Update();
  shader->SetUniformFromMemory(static_cast<unsigned int>(CommonUniform::Model),
                               GetModel());
  Texture* texture = nullptr;
  if ((texture = Get<Texture>()))
    texture->Use();
  for (std::size_t i = 0; i < mesh_container_.size(); i++) {
    if (mesh_to_texture_[i] < textures_container_.size() &&
        textures_container_[mesh_to_texture_[i]]) {
      textures_container_[mesh_to_texture_[i]]->Use();
    }
    mesh_container_[i]->Draw(render_target);
  }
}

void Model::Clear() {
  if (GetStatus() == Status::Loading) {
    LOG(ERROR) << "Resrouce is still loading!";
  }
  mesh_container_.clear();
  textures_container_.clear();
  mesh_to_texture_.clear();
}
