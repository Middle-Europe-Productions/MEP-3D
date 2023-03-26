#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <GL/glew.h>
#include <MEP-3D/color.hpp>
#include <MEP-3D/identity.hpp>
#include <MEP-3D/vector.hpp>
#include <string>
#include <vector>

namespace mep {
class Image : public Identity {
 public:
  enum class Type : int { RGBA = GL_RGBA, RGB = GL_RGB, Unknown = 0 };
  Image();
  Image(std::string_view name);
  Image(Image&& image);
  Image(const Image& image);
  bool LoadFromFile(const std::string& path);
  bool LoadFromMemory(const std::vector<Uint8>& data,
                      Vec2i size,
                      Type image_type);
  Uint8* GetPixels();
  const Uint8* GetPixels() const;
  Type GetType() const;
  const Vec2i& GetSize() const;
  Uint8& operator[](const Vec2i& index);
  const Uint8& operator[](const Vec2i& index) const;
  void Clear();

 private:
  std::vector<Uint8> image_data_;
  Vec2i size_;
  Type image_type_;
};
}  // namespace mep

#endif