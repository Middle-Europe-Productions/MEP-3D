#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <MEP-3D/image.hpp>
#include <MEP-3D/observer_list.hpp>
#include <MEP-3D/texture_base.hpp>

#include <memory>

namespace mep {
class Texture : public TextureBase {
 public:
  Texture();
  void Create(Color color);
  void Create(Image& image);
  void Use();
  void Stop();
  void Clear();
  virtual ~Texture();

 private:
  GLuint texture_id_;
};

using TexturePtr = std::unique_ptr<Texture>;
}  // namespace mep

#endif