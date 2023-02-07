#include <MEP-3D/texture_3d.hpp>

#include <MEP-3D/common_names.hpp>

Texture3D::Texture3D() : texture_id_(0), Asset(kTexture3d) {
  VLOG(4) << __func__;
}

void Texture3D::Create(const Uint8* data, Vec3i size, Type type) {
  glGenTextures(1, &texture_id_);
  if (texture_id_ == 0) {
    LOG(ERROR) << "Could not create a texture!";
    return;
  }
  glBindTexture(GL_TEXTURE_3D, texture_id_);

  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_3D, 0);
  VLOG(2) << "Transferring texture to gpu " << ToString();
  if (type == Type::BYTE_8) {
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, size.x_, size.y_, size.z_, 0, GL_RED,
                 GL_UNSIGNED_BYTE, data);

  } else {
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R16, size.x_, size.y_, size.z_, 0, GL_RED,
                 GL_UNSIGNED_BYTE, data);
  }
  glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::Use() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, texture_id_);
}

void Texture3D::Stop() {
  glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::Clear() {
  glDeleteTextures(1, &texture_id_);
  texture_id_ = 0;
}

bool Texture3D::IsValid() const {
  return texture_id_ != 0;
}

Texture3D::~Texture3D() {
  VLOG(4) << __func__;
  Clear();
}

std::string ToString(Texture3D::Type type) {
  switch (type) {
    case Texture3D::Type::BYTE_8:
      return "8 Byte";
    case Texture3D::Type::BYTE_16:
      return "16 Byte";
  }
  return "Unkown";
}
