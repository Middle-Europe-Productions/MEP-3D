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

void CameraBase::OnKeyEvent(KeyEvent event) {
  if (IsDisabled()) {
    return;
  }
  if (event.action == Action::Pressed || event.action == Action::Released) {
    if (ContainsKey(event.code)) {
      key_status_[event.code] = event.action == Action::Pressed;
    }
  }
}

void CameraBase::DisableEvents() {
  disabled_ = true;
}

void CameraBase::EnableEvents() {
  disabled_ = false;
}

bool CameraBase::IsDisabled() const {
  return disabled_;
}

glm::vec3 CameraBase::GetPosition() const {
  return Get(CameraVariables::Position);
}

glm::vec3 CameraBase::GetNormalizedDirection() const {
  return Get(CameraVariables::Direction);
}

bool CameraBase::ContainsKey(Key key) const {
  DCHECK(!controls_.empty());
  return key_status_.find(key) != key_status_.end();
}
void CameraBase::ResetCameraControls() {
  for (auto& it : key_status_) {
    it.second = false;
  }
}
