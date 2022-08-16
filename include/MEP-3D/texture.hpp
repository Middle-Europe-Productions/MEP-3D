#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <MEP-3D/asset.hpp>
#include <MEP-3D/asset_observer.hpp>
#include <MEP-3D/image.hpp>
#include <MEP-3D/observer_list.hpp>

#include <memory>

class Texture : public Asset {
 public:
  Texture();
  void Create(Image& image);
  void Use();
  void Stop();
  void Clear();
  virtual ~Texture();

 private:
  GLuint texture_id_;
};

using TexturePtr = std::unique_ptr<Texture>;

#endif