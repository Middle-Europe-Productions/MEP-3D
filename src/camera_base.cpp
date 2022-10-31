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

CameraBase::CameraBase() : Identity(kCamera) {}

CameraBase::CameraBase(const std::string& name)
    : Identity(kCamera, name.c_str()) {}
