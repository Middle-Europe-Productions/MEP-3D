#include <glog/logging.h>
#include <MEP-3D/model.hpp>

Model::Model() : Identity(__FUNCTION__) {
  LOG(INFO) << __FUNCTION__ << ", not implemented!";
}

Model::Model(const std::string& name) : Identity(__FUNCTION__, name.c_str()) {
  LOG(INFO) << __FUNCTION__ << ", not implemented!";
}

void Model::Load(const std::string& file_path) {
  LOG(INFO) << __FUNCTION__ << ", not implemented!";
}

void Model::Draw(RenderTarget& render_target) {
  LOG(INFO) << __FUNCTION__ << ", not implemented!";
}

void Model::Clear() {
  LOG(INFO) << __FUNCTION__ << ", not implemented!";
}

std::string Model::ToString() const {
  return Identity::ToString() + ", \n" + AssetController::ToString() + ", \n" +
         ModelController::ToString();
}