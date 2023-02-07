#ifndef TEXTURE_3D_HPP
#define TEXTURE_3D_HPP

#include <GL/glew.h>
#include <MEP-3D/asset.hpp>
#include <MEP-3D/common.hpp>
#include <MEP-3D/vector.hpp>

class Texture3D : public Asset {
 public:
  enum class Type { BYTE_8, BYTE_16 };
  Texture3D();
  void Create(const Uint8* data, Vec3i size, Type type = Type::BYTE_8);
  void Use();
  void Stop();
  void Clear();
  bool IsValid() const;
  virtual ~Texture3D();

 private:
  GLuint texture_id_;
};

std::string ToString(Texture3D::Type type);

#endif