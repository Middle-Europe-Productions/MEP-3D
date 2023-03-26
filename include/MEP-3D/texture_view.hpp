#ifndef TEXTURE_VIEW_HPP
#define TEXTURE_VIEW_HPP

#include <MEP-3D/texture_utils.hpp>

namespace mep {
class TextureView final {
 public:
  TextureView();
  TextureView(unsigned int handler);
  void Use(unsigned int slot);
  void Stop();
  void Clear();
  void SetHandler(unsigned int handler);
  unsigned int GetHandler();
  ~TextureView();

 public:
  unsigned int handler_;
};
};  // namespace mep

#endif