#include <glog/logging.h>
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
  LOG(ERROR) << __func__ << " is not implemented!";
  return false;
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
  DCHECK(index.x_ < size_.x_ && index.y_ < size_.y_);
  return image_data_[index.x_ + index.x_ * index.y_];
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
