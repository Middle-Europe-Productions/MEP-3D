#include <glog/logging.h>
#include <MEP-3D/model.hpp>

Model::Model() : Identity(__FUNCTION__) {
  UpdateStatus(Status::NotImplemented);
  LOG(WARNING) << __FUNCTION__ << ", not implemented!";
}

Model::Model(const std::string& name) : Identity(__FUNCTION__, name.c_str()) {
  UpdateStatus(Status::NotImplemented);
  LOG(WARNING) << __FUNCTION__ << ", not implemented!";
}

void Model::Load(const std::string&) {
  LOG(WARNING) << __FUNCTION__ << ", not implemented!";
}

void Model::Draw(RenderTarget&) {
  VLOG(3) << __FUNCTION__ << ", not implemented!";
}

void Model::Clear() {
  LOG(WARNING) << __FUNCTION__ << ", not implemented!";
}

std::string Model::ToString() const {
  return Identity::ToString() + ", \n" + AssetController::ToString() + ", \n" +
         ModelController::ToString();
}