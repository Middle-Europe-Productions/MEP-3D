#include <MEP-3D/camera_base.hpp>

std::string ToString(CameraVariables camera_variable) {
  switch (camera_variable) {
    case CameraVariables::Position:
      return "Position";
    case CameraVariables::Direction:
      return "Directin";
  }
  return "Unknown";
}

CameraBase::CameraBase(std::string_view identity) : Identity(identity) {}

CameraBase::CameraBase(std::string_view identity, std::string_view name)
    : Identity(identity, name) {}

glm::vec3 CameraBase::GetPosition() const {
  return Get(CameraVariables::Position);
}

glm::vec3 CameraBase::GetNormalizedDirection() const {
  return Get(CameraVariables::Direction);
}
