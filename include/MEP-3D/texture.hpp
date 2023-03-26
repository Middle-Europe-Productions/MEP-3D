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
  bool Create();
  bool Create(Color color);
  bool Update(Color color);
  bool Create(const Image& image);
  bool Update(const Image& image);
  void Use();
  void Stop();
  void Clear();
  unsigned int GetHandler();
  virtual ~Texture();

 private:
  GLuint texture_id_;
};

using TexturePtr = std::unique_ptr<Texture>;
}  // namespace mep

#endif