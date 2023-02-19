#include <MEP-3D/common_names.hpp>
#include <MEP-3D/perspective_camera.hpp>
#include <MEP-3D/utils.hpp>

std::string ToString(PerspectiveCameraActions pca) {
  switch (pca) {
    case PerspectiveCameraActions::Front:
      return "Front";
    case PerspectiveCameraActions::Back:
      return "Back";
    case PerspectiveCameraActions::Left:
      return "Left";
    case PerspectiveCameraActions::Right:
      return "Right";
    case PerspectiveCameraActions::Up:
      return "Up";
    case PerspectiveCameraActions::Down:
      return "Down";
    case PerspectiveCameraActions::Count:
    default:
      return "Count";
  }
}

PerspectiveCamera::PerspectiveCamera(
    const PerspectiveCameraConfig& config,
    const PerspectiveCameraControlsMap& controls)
    : CameraBase(kPerspectiveCamera) {
  InitCameraControls(PerspectiveCameraActions::Count, controls);
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
}

void PerspectiveCamera::Update(float time_delta) {
  ValidateKeyInput(time_delta);
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

void PerspectiveCamera::OnMouseEvent(MouseEvent event) {
  if (IsDisabled()) {
    return;
  }
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
  if (pitch_ > 89.9f) {
    pitch_ = 89.9f;
  }
  if (pitch_ < -89.9f) {
    pitch_ = -89.9f;
  }
  Changed();
}

void PerspectiveCamera::OnWindowResizeEvent(Vec2i) {}

void PerspectiveCamera::OnEventStatusChanged(bool events_blocked) {
  if (events_blocked == true) {
    ResetCameraControls();
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

void PerspectiveCamera::ValidateKeyInput(float time_delta) {
  bool pressed = false;
  float velocity = move_speed_ * time_delta;
  if (IsActive(PerspectiveCameraActions::Front)) {
    Increment(CameraVariables::Position,
              Get(CameraVariables::Direction) * velocity);
    pressed = true;
  }
  if (IsActive(PerspectiveCameraActions::Back)) {
    Decrement(CameraVariables::Position,
              Get(CameraVariables::Direction) * velocity);
    pressed = true;
  }
  if (IsActive(PerspectiveCameraActions::Left)) {
    Decrement(CameraVariables::Position, right_ * velocity);
    pressed = true;
  }
  if (IsActive(PerspectiveCameraActions::Right)) {
    Increment(CameraVariables::Position, right_ * velocity);
    pressed = true;
  }
  if (IsActive(PerspectiveCameraActions::Up)) {
    Get(CameraVariables::Position)[1] += velocity;
    pressed = true;
  }
  if (IsActive(PerspectiveCameraActions::Down)) {
    Get(CameraVariables::Position)[1] -= velocity;
    pressed = true;
  }
  if (pressed)
    Changed();
}
