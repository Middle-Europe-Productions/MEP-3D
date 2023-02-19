#ifndef TEXTURE_BASE_HPP
#define TEXTURE_BASE_HPP

#include <MEP-3D/asset.hpp>

class TextureBase : public Asset {
 public:
  TextureBase(const char* identity) : Asset(identity) {}
  TextureBase(const char* identity, const char* name) : Asset(identity, name) {}
  virtual void Use() = 0;
  virtual void Stop() = 0;
  virtual void Clear() = 0;
  virtual ~TextureBase() = default;
};

#endif