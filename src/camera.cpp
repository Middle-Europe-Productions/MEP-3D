#include <MEP-3D/camera.hpp>

Camera::Camera(const CameraConfig& config, const CameraControls& controls)
    : controls_(controls), camera_time_delta_(TimeDelta::GetInstance()) {
  position_ = config.start_position;
  world_up_ = config.start_up;
  yaw_ = config.start_yaw;
  pitch_ = config.start_pitch;
  front_ = glm::vec3(0.0f, 0.0f, -1.0f);
  move_speed_ = config.start_move_speed;
  turn_speed_ = config.start_turn_speed;
  last_mouse_x_ = 0.f;
  last_mouse_y_ = 0.f;
  mouse_x_change = 0.f;
  mouse_y_change = 0.f;
  initial_move_ = true;
  Update();
}

void Camera::Update() {
  ValidateKeyboardInput();
  UpdateInt([this]() {
    front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_.y = sin(glm::radians(pitch_));
    front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front_);

    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
  });
}

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(position_, position_ + front_, up_);
}

glm::vec3 Camera::GetPosition() const {
  return position_;
}

glm::vec3 Camera::GetNormalizedDirection() const
{
    return glm::normalize(front_);
}

void Camera::OnKeyEvent(KeyEvent event) {
  if (event.action == Action::Pressed || event.action == Action::Released) {
    if (event.code == controls_.up || event.code == controls_.down ||
        event.code == controls_.left || event.code == controls_.right) {
      key_status_[event.code] = event.action == Action::Pressed;
    }
  }
}

void Camera::OnMouseEvent(MouseEvent event) {
  if (initial_move_) {
    last_mouse_x_ = event.x;
    last_mouse_y_ = event.y;
    initial_move_ = false;
  } else if (last_mouse_x_ == event.x && last_mouse_y_ == event.y) {
    return;
  }
  mouse_x_change = event.x - last_mouse_x_;
  mouse_y_change = event.y - last_mouse_y_;

  last_mouse_x_ = event.x;
  last_mouse_y_ = event.y;

  mouse_x_change *= turn_speed_;
  mouse_y_change *= turn_speed_;
  yaw_ += mouse_x_change;
  pitch_ += mouse_y_change;
  if (pitch_ > 89.9f) {
    pitch_ = 89.9f;
  }
  if (pitch_ > -89.9f) {
    pitch_ = -89.9f;
  }
  Changed();
}

std::string Camera::ToString() const {
  return std::string("camera: { \nPosition: " + glm::to_string(position_) +
                     ", \n" + "Front: " + glm::to_string(front_) + ", \n" +
                     "Up: " + glm::to_string(up_) + ", \n" +
                     "Right: " + glm::to_string(right_) + ", \n" +
                     "World_up: " + glm::to_string(world_up_) + ", \n" +
                     "Yaw: " + std::to_string(yaw_) + ", \n" +
                     "Pitch: " + std::to_string(pitch_) + "\n}");
}

void Camera::InitKeyboardMap() {
  key_status_[controls_.up] = false;
  key_status_[controls_.down] = false;
  key_status_[controls_.left] = false;
  key_status_[controls_.right] = false;
}

void Camera::ValidateKeyboardInput() {
  bool pressed = false;
  double velocity = move_speed_ * camera_time_delta_->GetTimeDelta();
  if (key_status_[controls_.up]) {
    position_ += front_ * (GLfloat)velocity;
    pressed = true;
  }
  if (key_status_[controls_.down]) {
    position_ -= front_ * (GLfloat)velocity;
    pressed = true;
  }
  if (key_status_[controls_.left]) {
    position_ -= right_ * (GLfloat)velocity;
    pressed = true;
  }
  if (key_status_[controls_.right]) {
    position_ += right_ * (GLfloat)velocity;
    pressed = true;
  }
  if (pressed)
    Changed();
}