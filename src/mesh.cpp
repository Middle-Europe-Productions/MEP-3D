#include <MEP-3D/mesh.hpp>
namespace {
constexpr int kSizeOfVertice = sizeof(GLfloat);
constexpr int kSizeOfIndice = sizeof(unsigned int);
constexpr int kRowSize = 8;
}  // namespace

Mesh::Mesh() {
  LOG(INFO) << __FUNCTION__;
  vertices_count_ = 0;
}

void Mesh::Clear() {
  MeshBase::Clear();
  vertices_count_ = 0;
}

void Mesh::Init(const std::vector<GLfloat>& vertices,
                const std::vector<unsigned int> indices) {
  vertices_count_ = vertices.size();
  glGenVertexArrays(1, &vertex_array_object_);
  glBindVertexArray(vertex_array_object_);

  glGenBuffers(1, &index_buffer_boject_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_boject_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, kSizeOfIndice * indices.size(),
               &indices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenBuffers(1, &vertex_buffer_object_);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * kSizeOfVertice, &vertices[0],
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, kSizeOfVertice * kRowSize, 0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, kSizeOfVertice * kRowSize,
                        (void*)(kSizeOfVertice * 3));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, kSizeOfVertice * kRowSize,
                        (void*)(kSizeOfVertice * 5));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::Draw(RenderTarget& render_target) {
  if (!Get<Shader>()) {
    return;
  }
  Shader* shader = Get<Shader>();
  Update();
  shader->SetUniformFromMemory(CommonUniform::Model, GetModel());
  if (render_target.GetView())
    shader->SetUniformFromMemory(CommonUniform::Projection,
                                 render_target.GetView()->GetProjection());
  if (render_target.GetCamera()) {
    shader->SetUniformFromMemory(CommonUniform::View,
                                 render_target.GetCamera()->GetViewMatrix());
    shader->SetUniformFromMemory(CommonUniform::Position,
                                 render_target.GetCamera()->GetPosition());
  }

  Texture* texture = nullptr;
  if ((texture = Get<Texture>()))
    texture->Use();

  glBindVertexArray(vertex_array_object_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_boject_);
  glDrawElements(GL_TRIANGLES, GetVerticesCount(), GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  if (texture)
    texture->Stop();
}

unsigned int Mesh::GetVerticesCount() const {
  return vertices_count_;
}

std::string Mesh::ToString() const {
  return "Mesh: {\n" + AssetController::ToString() + ",\n" +
         ModelController::ToString() + "\n}";
}
