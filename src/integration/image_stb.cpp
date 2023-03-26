#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glog/logging.h>
#include <MEP-3D/common.hpp>
#include <MEP-3D/common_names.hpp>
#include <MEP-3D/image.hpp>

namespace mep {
Image::Image() : Identity(kImage) {}

Image::Image(std::string_view name) : Identity(kImage, name) {}

Image::Image(Image&& image)
    : Identity(std::move(image)),
      image_data_(std::move(image.image_data_)),
      size_(image.size_),
      image_type_(image.image_type_) {}

Image::Image(const Image& image)
    : Identity(image),
      image_data_(image.image_data_),
      size_(image.size_),
      image_type_(image.image_type_) {}

bool Image::LoadFromFile(const std::string& name) {
  std::string path = kDefaultTexturePath + name;
  int bit_depth;
  Uint8* pixels = stbi_load(path.c_str(), &size_.x_, &size_.y_, &bit_depth, 0);
  if (!pixels) {
    LOG(ERROR) << "Could not open " << name << ", path: " << path;
    return false;
  }
  image_data_ =
      std::vector<Uint8>(pixels, pixels + size_.x_ * size_.y_ * bit_depth);
  if (bit_depth == 4) {
    image_type_ = Type::RGBA;
  } else if (bit_depth == 3) {
    image_type_ = Type::RGB;
  } else {
    image_type_ = Type::Unknown;
    LOG(WARNING) << "Unknown bit depth: " << name;
  }
  stbi_image_free(pixels);
  VLOG(3) << __func__ << ", size: " << image_data_.size()
          << ", type: " << static_cast<int>(image_type_);
  return true;
}

bool Image::LoadFromMemory(const std::vector<Uint8>& data,
                           Vec2i size,
                           Type image_type) {
  image_data_ = data;
  size_ = size;
  image_type_ = image_type;
  VLOG(3) << __func__ << ", size: " << image_data_.size()
          << ", type: " << static_cast<int>(image_type_);
  return true;
}

Uint8* Image::GetPixels() {
  return &image_data_[0];
}

const Uint8* Image::GetPixels() const {
  return &image_data_[0];
}

Uint8& Image::operator[](const Vec2i& index) {
  std::size_t index_norm = index.x_ + index.x_ * index.y_;
  DCHECK(index_norm <= image_data_.size());
  return image_data_[index_norm];
}

const Uint8& Image::operator[](const Vec2i& index) const {
  return (*this)[index];
}

Image::Type Image::GetType() const {
  return image_type_;
}

void Image::Clear() {
  image_data_.clear();
  size_ = {0, 0};
  image_type_ = Type::Unknown;
}

const Vec2i& Image::GetSize() const {
  return size_;
}
}  // namespace mep
