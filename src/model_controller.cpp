#include <glog/logging.h>
#include <MEP-3D/model_controller.hpp>
#include <MEP-3D/utils.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

namespace mep {
ModelController::ModelController() {
  VLOG(3) << __FUNCTION__;
  ResetModel();
}

void ModelController::Transform(const Vec3f& vec) {
  model_ = glm::translate(model_, glm::vec3(vec.x_, vec.y_, vec.z_));
}

void ModelController::Transform(float x, float y, float z) {
  model_ = glm::translate(model_, glm::vec3(x, y, z));
}

void ModelController::Rotate(const Vec3f& vec) {
  rotation_tracker_deg_[0] += utils::RadiansToDeg(vec.x_);
  rotation_tracker_deg_[1] += utils::RadiansToDeg(vec.y_);
  rotation_tracker_deg_[2] += utils::RadiansToDeg(vec.z_);
  model_ = glm::translate(model_, glm::vec3(vec.x_, vec.y_, vec.z_));
}

void ModelController::Rotate(float degrees, Axis axis) {
  switch (axis) {
    case Axis::X:
      rotation_tracker_deg_[0] += degrees;
      break;
    case Axis::Y:
      rotation_tracker_deg_[1] += degrees;
      break;
    case Axis::Z:
      rotation_tracker_deg_[2] += degrees;
      break;
  }
  model_ =
      glm::rotate(model_, utils::DegToRadians(degrees), utils::AxisToGlm(axis));
}

void ModelController::Scale(const Vec3f& val) {
  model_ = glm::scale(model_, glm::vec3(val.x_, val.y_, val.z_));
}

void ModelController::Scale(float x, float y, float z) {
  model_ = glm::scale(model_, glm::vec3(x, y, z));
}

void ModelController::ResetModel() {
  model_ = glm::mat4(1.0f);
  rotation_tracker_deg_ = glm::vec3(0.0, 0.0, 0.0);
}

glm::mat4& ModelController::GetModel() {
  return model_;
}

const glm::mat4& ModelController::GetModel() const {
  return model_;
}

const glm::vec3& ModelController::GetRotationTracker() const {
  return rotation_tracker_deg_;
}

std::string ModelController::ToString() const {
  return "\"ObjectActionController\": {\n\"model_matrix\": " +
         glm::to_string(model_) + "\n}";
}
}  // namespace mep
