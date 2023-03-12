#include <glog/logging.h>
#include <MEP-3D/image.hpp>

namespace mep {
Image::Image() {
  LOG(INFO) << "Default implementation of " << __FUNCTION__;
}

bool Image::LoadFromFile(const std::string& name) {
  LOG(INFO) << "Default implementation of " << __FUNCTION__;
  return false;
}

Uint8* Image::GetPixels() {
  return nullptr;
}
}  // namespace mep
