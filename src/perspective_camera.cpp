#include <MEP-3D/perspective_camera.hpp>

std::string ToString(const CameraVariables& camera_variable) {
  switch (camera_variable) {
    case CameraVariables::Position:
      return "Position";
    case CameraVariables::Direction:
      return "Directin";
  }
  return "Unknown";
}

PerspectiveCamera::PerspectiveCamera(const CameraConfig& config,
                                     const PerspectiveCameraControls& controls)
    : controls_(controls), camera_time_delta_(TimeDelta::GetInstance()) {
  Set(CameraVariables::Position, config.start_position);
  Set(CameraVariables::Direction, glm::vec3(0.0f, 0.0f, -1.0f));
  world_up_ = config.start_up;
  yaw_ = config.start_yaw;
  pitch_ = config.start_pitch;
  move_speed_ = config.start_move_speed;
  turn_speed_ = config.start_turn_speed;
  last_mouse_x_ = 0.f;
  last_mouse_y_ = 0.f;
  mouse_x_change = 0.f;
  mouse_y_change = 0.f;
  initial_move_ = true;
  Update();
}

void PerspectiveCamera::Update() {
  ValidateKeyboardInput();
  UpdateInt([this]() {
    Set(CameraVariables::Direction,
        glm::normalize(
            glm::vec3(cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)),
                      sin(glm::radians(pitch_)),
                      sin(glm::radians(yaw_)) * cos(glm::radians(pitch_)))));

    right_ =
        glm::normalize(glm::cross(Get(CameraVariables::Direction), world_up_));
    up_ = glm::normalize(glm::cross(right_, Get(CameraVariables::Direction)));
  });
}

glm::mat4 PerspectiveCamera::GetViewMatrix() const {
  return glm::lookAt(
      Get(CameraVariables::Position),
      Get(CameraVariables::Position) + Get(CameraVariables::Direction), up_);
}

glm::vec3 PerspectiveCamera::GetPosition() const {
  return Get(CameraVariables::Position);
}

glm::vec3 PerspectiveCamera::GetNormalizedDirection() const {
  return Get(CameraVariables::Direction);
}

void PerspectiveCamera::OnKeyEvent(KeyEvent event) {
  if (event.action == Action::Pressed || event.action == Action::Released) {
    if (event.code == controls_.up || event.code == controls_.down ||
        event.code == controls_.left || event.code == controls_.right) {
      key_status_[event.code] = event.action == Action::Pressed;
    }
  }
}

void PerspectiveCamera::OnMouseEvent(MouseEvent event) {
  if (initial_move_) {
    last_mouse_x_ = event.x;
    last_mouse_y_ = event.y;
    initial_move_ = false;
  } else if (last_mouse_x_ == event.x && last_mouse_y_ == event.y) {
    return;
  }
  if (reversed_x_axis_) {
    mouse_x_change = last_mouse_x_ - event.x;
  } else {
    mouse_x_change = event.x - last_mouse_x_;
  }
  if (reversed_y_axis_) {
    mouse_y_change = event.y - last_mouse_y_;
  } else {
    mouse_y_change = last_mouse_y_ - event.y;
  }

  last_mouse_x_ = event.x;
  last_mouse_y_ = event.y;

  mouse_x_change *= turn_speed_;
  mouse_y_change *= turn_speed_;
  yaw_ += mouse_x_change;
  pitch_ += mouse_y_change;
  LOG(INFO) << pitch_ << " " << yaw_;
  if (pitch_ > 89.9f) {
    LOG(INFO) << __LINE__;
    pitch_ = 89.9f;
  }
  if (pitch_ < -89.9f) {
    LOG(INFO) << __LINE__;
    pitch_ = -89.9f;
  }
  Changed();
}

void PerspectiveCamera::OnWindowResizeEvent(Vec2i size) {}

void PerspectiveCamera::OnEventStatusChanged(bool events_blocked) {
  if (events_blocked == true) {
    InitKeyboardMap();
  }
  initial_move_ = true;
}

std::string PerspectiveCamera::ToString() const {
  return std::string(
      "\"camera\": { \n\"Position\": " +
      glm::to_string(Get(CameraVariables::Position)) + ", \n" +
      "\"Direction\": " + glm::to_string(Get(CameraVariables::Direction)) +
      ", \n" + "\"Up\": " + glm::to_string(up_) + ", \n" +
      "\"Right\": " + glm::to_string(right_) + ", \n" + "\"World_up\": " +
      glm::to_string(world_up_) + ", \n" + "\"Yaw\": " + std::to_string(yaw_) +
      ", \n" + "\"Pitch\": " + std::to_string(pitch_) + "\n}");
}

void PerspectiveCamera::InitKeyboardMap() {
  key_status_[controls_.up] = false;
  key_status_[controls_.down] = false;
  key_status_[controls_.left] = false;
  key_status_[controls_.right] = false;
}

void PerspectiveCamera::ValidateKeyboardInput() {
  bool pressed = false;
  double velocity = move_speed_ * camera_time_delta_->GetTimeDelta();
  if (key_status_[controls_.up]) {
    Increment(CameraVariables::Position,
              Get(CameraVariables::Direction) * (GLfloat)velocity);
    pressed = true;
  }
  if (key_status_[controls_.down]) {
    Decrement(CameraVariables::Position,
              Get(CameraVariables::Direction) * (GLfloat)velocity);
    pressed = true;
  }
  if (key_status_[controls_.left]) {
    Decrement(CameraVariables::Position, right_ * (GLfloat)velocity);
    pressed = true;
  }
  if (key_status_[controls_.right]) {
    Increment(CameraVariables::Position, right_ * (GLfloat)velocity);
    pressed = true;
  }
  if (pressed)
    Changed();
}