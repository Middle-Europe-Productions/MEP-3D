#include <MEP-3D/common.hpp>
#include <MEP-3D/figures.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mep {
namespace {
constexpr unsigned int kRowSize = 8;
const std::vector<unsigned int> kPyramidIndicies = {0, 3, 1, 1, 3, 2,
                                                    2, 3, 0, 0, 1, 2};
const std::vector<unsigned int> kPlaneIndices = {0, 2, 1, 1, 2, 3};
const std::vector<unsigned int> kTriangleIndices = {0, 1, 2};

const std::vector<unsigned int> kCubeIndices = {
    // bottom face
    0, 2, 3, 3, 1, 0,
    // top face
    4, 6, 7, 7, 5, 4,
    // front face
    8, 10, 11, 11, 9, 8,
    // back face
    12, 14, 15, 15, 13, 12,
    // left face
    16, 17, 18, 18, 19, 17,
    // right face
    20, 22, 23, 23, 21, 20};

void CalculateNormals(std::vector<GLfloat>& vertice_array,
                      unsigned int normal_offset) {
  for (std::size_t i = 0; i < kPyramidIndicies.size(); i += 3) {
    unsigned int i_0 = kPyramidIndicies[i] * kRowSize;
    unsigned int i_1 = kPyramidIndicies[i + 1] * kRowSize;
    unsigned int i_2 = kPyramidIndicies[i + 2] * kRowSize;

    glm::vec3 v1(vertice_array[i_1] - vertice_array[i_0],
                 vertice_array[i_1 + 1] - vertice_array[i_0 + 1],
                 vertice_array[i_1 + 2] - vertice_array[i_0 + 2]);
    glm::vec3 v2(vertice_array[i_2] - vertice_array[i_0],
                 vertice_array[i_2 + 1] - vertice_array[i_0 + 1],
                 vertice_array[i_2 + 2] - vertice_array[i_0 + 2]);
    glm::vec3 normal = glm::cross(v1, v2);
    normal = glm::normalize(normal);

    i_0 += normal_offset;
    i_1 += normal_offset;
    i_2 += normal_offset;
    vertice_array[i_0] += normal.x;
    vertice_array[i_0 + 1] += normal.y;
    vertice_array[i_0 + 2] += normal.z;
    vertice_array[i_1] += normal.x;
    vertice_array[i_1 + 1] += normal.y;
    vertice_array[i_1 + 2] += normal.z;
    vertice_array[i_2] += normal.x;
    vertice_array[i_2 + 1] += normal.y;
    vertice_array[i_2 + 2] += normal.z;
  }
  for (std::size_t i = 0; i < vertice_array.size() / kRowSize; ++i) {
    unsigned int normalize_offset = i * kRowSize + normal_offset;
    glm::vec3 vec = normalize(glm::vec3(vertice_array[normalize_offset],
                                        vertice_array[normalize_offset + 1],
                                        vertice_array[normalize_offset + 2]));
    vertice_array[normalize_offset] = vec.x;
    vertice_array[normalize_offset + 1] = vec.y;
    vertice_array[normalize_offset + 2] = vec.z;
  }
}

}  // namespace

Pyramid::Pyramid(Vec3f initial_position) {
  VLOG(3) << __FUNCTION__;
  std::vector<GLfloat> vertices = {
      // x,     y,    z,    u,   v,     nx,   ny,   nz
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 1.0f,
      0.5f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  0.0f,
      0.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.5f, 1.0f,  0.0f, 0.0f, 0.0f};
  CalculateNormals(vertices, 5);
  Mesh::Init(vertices, kPyramidIndicies);
  Transform(initial_position);
}

Plane::Plane(float size, Vec3f initial_position) {
  VLOG(3) << __FUNCTION__;
  std::vector<GLfloat> vertices = {
      // x, y, z, u, v, nx, ny, nx
      -size, 0.0f, -size, 0.0f,  0.0f, 0.0f,  -1.0f, 0.0f, size,  0.0f, -size,
      size,  0.0f, 0.0f,  -1.0f, 0.0f, -size, 0.0f,  size, 0.0f,  size, 0.0f,
      -1.0f, 0.0f, size,  0.0f,  size, size,  size,  0.0f, -1.0f, 0.0f};
  Mesh::Init(vertices, kPlaneIndices);
  Transform(initial_position);
}

Cube::Cube(float size, Vec3f initial_position) {
  VLOG(3) << __FUNCTION__;
  std::vector<GLfloat> vertices = {
      // x, y, z, u, v, nx, ny, nz
      // bottom face
      -size, 0.0f, -size, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, size, 0.0f, -size,
      size, 0.0f, 0.0f, -1.0f, 0.0f, -size, 0.0f, size, 0.0f, size, 0.0f, -1.0f,
      0.0f, size, 0.0f, size, size, size, 0.0f, -1.0f, 0.0f,
      // top face
      -size, 2 * size, -size, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, size, 2 * size,
      -size, size, 0.0f, 0.0f, 1.0f, 0.0f, -size, 2 * size, size, 0.0f, size,
      0.0f, 1.0f, 0.0f, size, 2 * size, size, size, size, 0.0f, 1.0f, 0.0f,
      // front face
      -size, 0.0f, size, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, size, 0.0f, size, size,
      0.0f, 0.0f, 0.0f, 1.0f, -size, 2 * size, size, 0.0f, 2 * size, 0.0f, 0.0f,
      1.0f, size, 2 * size, size, size, 2 * size, 0.0f, 0.0f, 1.0f,
      // back face
      -size, 0.0f, -size, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, size, 0.0f, -size,
      size, 0.0f, 0.0f, 0.0f, -1.0f, -size, 2 * size, -size, 0.0f, 2 * size,
      0.0f, 0.0f, -1.0f, size, 2 * size, -size, size, 2 * size, 0.0f, 0.0f,
      -1.0f,
      // left face
      -size, 0.0f, size, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -size, 2 * size, size,
      0.0f, 2 * size, -1.0f, 0.0f, 0.0f, -size, 0.0f, -size, size, 0.0f, -1.0f,
      0.0f, 0.0f, -size, 2 * size, -size, size, 2 * size, -1.0f, 0.0f, 0.0f,
      // right face
      size, 0.0f, size, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, size, 2 * size, size,
      0.0f, 2 * size, 1.0f, 0.0f, 0.0f, size, 0.0f, -size, size, 0.0f, 1.0f,
      0.0f, 0.0f, size, 2 * size, -size, size, 2 * size, 1.0f, 0.0f, 0.0f};

  Mesh::Init(vertices, kCubeIndices);
  Transform(initial_position);
}

Triangle::Triangle(Vec3f initial_position) {
  VLOG(3) << __FUNCTION__;
  std::vector<GLfloat> vertices = {
      // x,     y,    z,    u,   v,     nx,   ny,   nz
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f,
      0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
  };
  Mesh::Init(vertices, kTriangleIndices);
  Transform(initial_position);
}
}  // namespace mep
