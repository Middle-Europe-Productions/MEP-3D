#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>

#include<MEP-3D/image.hpp>
#include<glog/logging.h>

Image::Image() {}

bool Image::LoadFromFile(const std::string& name) {
    Uint8* pixels = stbi_load(name, &width_, &height_, &bit_depth_, 0);
    if (!pixels) {
        LOG(ERROR) << "Could not open " << name;
        return false;
    }
    LOG(INFO) << "Width: " << width_ << ", height: "<< height_ << ", depth: " << bit_depth_;
    image_data_ = std::vector<Uint8>(pixels, pixels + width_ * height_);
}

Uint8* Image::GetPixels() {
    return &image_data_[0];
}
