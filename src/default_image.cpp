#include <glog/logging.h>
#include <MEP-3D/image.hpp>


Image::Image() {
  LOG(INFO) << "Default implementation of " << __FUNCTION__;
}

bool Image::LoadFromFile(const std::string& name) {
  LOG(INFO) << "Default implementation of " << __FUNCTION__;
}

Uint8* Image::GetPixels() {
  return nullptr;
}
