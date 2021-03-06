#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

#include<MEP-3D/image.hpp>
#include<glog/logging.h>

Image::Image() {}

bool Image::LoadFromFile(const std::string& name) {
    Uint8* pixels = stbi_load(name.c_str(), &size_.x_, &size_.y_, &bit_depth_, 0);
    if (!pixels) {
        LOG(ERROR) << "Could not open " << name;
        return false;
    }
    image_data_ = std::vector<Uint8>(pixels, pixels + size_.x_ * size_.y_ * bit_depth_);
    if (bit_depth_ == 4) {
        image_type_ = Type::RGBA;
    } else if (bit_depth_ == 3) {
        image_type_ = Type::RGB;
    } else {
        image_type_ = Type::Unknown;
        LOG(WARNING) << "Unknown bit depth: " << name;
    }
    stbi_image_free(pixels);
}

Uint8* Image::GetPixels() {
    return &image_data_[0];
}

Image::Type Image::GetType() const {
    return image_type_;
}

void Image::Clear() {
    image_data_.clear();
    size_ = { 0, 0 };
    bit_depth_ = 0;
    image_type_ = Type::Unknown;
}

const Vec2i& Image::GetSize() const {
    return size_;
}