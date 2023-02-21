#ifndef MESH_BASE_HPP
#define MESH_BASE_HPP

#include <GL/glew.h>
#include <MEP-3D/drawable.hpp>
#include <memory>

namespace mep {
class MeshBase : public Drawable {
 public:
  MeshBase();
  void Clear();
  virtual void Init(const std::vector<GLfloat>& vertices,
                    const std::vector<unsigned int>& indices,
                    bool position_only = false);
  virtual void Draw(RenderTarget&);
  unsigned int GetVerticesCount() const;
  virtual ~MeshBase();

 protected:
  GLuint vertex_buffer_object_;
  GLuint vertex_array_object_;
  GLuint index_buffer_object_;
  unsigned int vertices_count_;
};
using MeshBasePtr = std::unique_ptr<MeshBase>;

class MeshBaseFactory {
 public:
  MeshBaseFactory(const std::vector<GLfloat>& vertices,
                  const std::vector<unsigned int> indices);
  MeshBaseFactory(std::vector<GLfloat>&& vertices,
                  std::vector<unsigned int>&& indices);
  MeshBasePtr Create();

 private:
  std::vector<GLfloat> vertices_;
  std::vector<unsigned int> indices_;
};
using MeshBaseFactoryPtr = std::unique_ptr<MeshBaseFactory>;
}  // namespace mep

#endif
