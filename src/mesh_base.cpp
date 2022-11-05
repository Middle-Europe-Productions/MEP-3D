#include <MEP-3D/mesh_base.hpp>

namespace {
constexpr int kSizeOfVertice = sizeof(GLfloat);
constexpr int kSizeOfIndice = sizeof(unsigned int);
constexpr int kRowSize = 8;
}  // namespace

MeshBase::MeshBase() {
  vertex_buffer_object_ = 0;
  vertex_array_object_ = 0;
  index_buffer_object_ = 0;
}

void MeshBase::Init(const std::vector<GLfloat>& vertices,
                    const std::vector<unsigned int>& indices) {
  vertices_count_ = vertices.size();
  glGenVertexArrays(1, &vertex_array_object_);
  glBindVertexArray(vertex_array_object_);

  glGenBuffers(1, &index_buffer_object_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
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

void MeshBase::Draw(RenderTarget& render_target) {
  glBindVertexArray(vertex_array_object_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
  glDrawElements(GL_TRIANGLES, GetVerticesCount(), GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

unsigned int MeshBase::GetVerticesCount() const {
  return vertices_count_;
}

void MeshBase::Clear() {
  if (vertex_array_object_ != 0) {
    glDeleteVertexArrays(1, &vertex_array_object_);
    vertex_array_object_ = 0;
  }
  if (vertex_buffer_object_ != 0) {
    glDeleteBuffers(1, &vertex_buffer_object_);
    vertex_buffer_object_ = 0;
  }
  if (index_buffer_object_ != 0) {
    glDeleteBuffers(1, &index_buffer_object_);
    index_buffer_object_ = 0;
  }
}

MeshBase::~MeshBase() {
  Clear();
}

MeshBaseFactory::MeshBaseFactory(const std::vector<GLfloat>& vertices,
                                 const std::vector<unsigned int> indices)
    : vertices_(vertices), indices_(indices) {}

MeshBaseFactory::MeshBaseFactory(std::vector<GLfloat>&& vertices,
                                 std::vector<unsigned int>&& indices)
    : vertices_(std::move(vertices)), indices_(std::move(indices)) {}

MeshBasePtr MeshBaseFactory::Create() {
  auto obj = std::make_unique<MeshBase>();
  obj->Init(vertices_, indices_);
  return std::move(obj);
}
