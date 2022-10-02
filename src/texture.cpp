#include <glog/logging.h>
#include <stb_image.h>
#include <MEP-3D/common_names.hpp>
#include <MEP-3D/texture.hpp>

namespace {
constexpr int kDefaultResolution = 16;
}

Texture::Texture() : Asset(kTexture) {
  texture_id_ = 0;
}

void Texture::Create(Color color) {
  VLOG(3) << __FUNCTION__ << ", " << ToString();
  glGenTextures(1, &texture_id_);
  if (texture_id_ == 0) {
    LOG(ERROR) << "Could not create a texture!";
    return;
  }
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  std::vector<Uint8> pixel_data(kDefaultResolution * kDefaultResolution * 4);
  for (int i = 0; i < kDefaultResolution * kDefaultResolution; ++i) {
    pixel_data[i] = color.R();
    pixel_data[i + 1] = color.G();
    pixel_data[i + 2] = color.B();
    pixel_data[i + 3] = color.A();
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, kDefaultResolution,
               kDefaultResolution, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               pixel_data.data());
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Create(Image& image) {
  VLOG(3) << __FUNCTION__ << ", " << ToString();
  glGenTextures(1, &texture_id_);
  if (texture_id_ == 0) {
    LOG(ERROR) << "Could not create a texture!";
    return;
  }
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, (GLint)image.GetType(), image.GetSize().x_,
               image.GetSize().y_, 0, (GLint)image.GetType(), GL_UNSIGNED_BYTE,
               image.GetPixels());
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Use() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void Texture::Stop() {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Clear() {
  glDeleteTextures(1, &texture_id_);
  texture_id_ = 0;
}

Texture::~Texture() {
  LOG(INFO) << __FUNCTION__ << ", " << ToString();
  Clear();
}
