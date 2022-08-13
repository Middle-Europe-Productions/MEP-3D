#include <glog/logging.h>
#include <MEP-3D/model_controller.hpp>
#include <MEP-3D/utils.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

ModelController::ModelController() {
  LOG(INFO) << __FUNCTION__;
  ResetModel();
}

void ModelController::Transform(const Vec3f& vec) {
  model_ = glm::translate(model_, glm::vec3(vec.x_, vec.y_, vec.z_));
}

void ModelController::Transform(float x, float y, float z) {
  model_ = glm::translate(model_, glm::vec3(x, y, z));
}

void ModelController::Rotate(const Vec3f& vec) {
  model_ = glm::translate(model_, glm::vec3(vec.x_, vec.y_, vec.z_));
}

void ModelController::Rotate(float degrees, Axis axis) {
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
}

glm::mat4& ModelController::GetModel() {
  return model_;
}

std::string ModelController::ToString() const {
  return "ObjectActionController: {\nmodel_matrix: " + glm::to_string(model_) +
         "\n}";
}
