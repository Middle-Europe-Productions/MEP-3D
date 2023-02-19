#include <MEP-3D/arcball_camera.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace {
constexpr float kPIval = 3.14159f;
template <typename T>
int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}
};  // namespace

ArcballCamera::ArcballCamera(const ArcballCameraConfig& config,
                             const ArcballCameraControlsMap& controls)
    : CameraBase(kArcballCamera) {
  InitCameraControls(ArcballCameraActions::Count, controls);
  Set(CameraVariables::Position, config.start_position);
  lookat_position_ = config.lookat_position;
  world_up_ = config.start_up;
  screen_size_ = config.initial_viewport_size;
  move_speed_ = config.move_speed;
}

void ArcballCamera::Update(float) {
  UpdateInt([this]() {
    glm::vec4 position(Get(CameraVariables::Position), 1);
    glm::vec4 pivot(lookat_position_, 1);
    float delta_angle_x = (2 * kPIval / screen_size_.x_);
    float delta_angle_y = (kPIval / screen_size_.y_);
    float angle_x = mouse_change_.x_ * delta_angle_x * move_speed_;
    float angle_y = mouse_change_.y_ * delta_angle_y * move_speed_;

    float cosAngle = dot(GetViewDir(), world_up_);
    if (cosAngle * sgn(angle_y) > 0.99f) {
      angle_y = 0;
    }

    glm::mat4x4 roatation_matrix_x(1.0f);
    roatation_matrix_x = glm::rotate(roatation_matrix_x, angle_x, world_up_);
    position = (roatation_matrix_x * (position - pivot)) + pivot;

    glm::mat4x4 rotation_matrix_y(1.0f);
    rotation_matrix_y =
        glm::rotate(rotation_matrix_y, angle_y, GetRightVector());

    Set(CameraVariables::Position,
        rotation_matrix_y * (position - pivot) + pivot);

    view_matrix_ = glm::lookAt(Get(CameraVariables::Position), lookat_position_,
                               world_up_);
  });
}

void ArcballCamera::OnKeyEvent(KeyEvent event) {
  if (!IsActive(ArcballCameraActions::Grab)) {
    initial_move_ = true;
  }
  CameraBase::OnKeyEvent(event);
}

glm::mat4 ArcballCamera::GetViewMatrix() const {
  return view_matrix_;
}

void ArcballCamera::OnMouseEvent(MouseEvent event) {
  if (!IsActive(ArcballCameraActions::Grab)) {
    return;
  }
  if (!initial_move_ && last_mouse_position_.x_ == event.x &&
      last_mouse_position_.y_ == event.y) {
    return;
  }
  if (!initial_move_) {
    mouse_change_.x_ = last_mouse_position_.x_ - event.x;
    mouse_change_.y_ = last_mouse_position_.y_ - event.y;
  }
  initial_move_ = false;
  last_mouse_position_.x_ = event.x;
  last_mouse_position_.y_ = event.y;
  Changed();
}

void ArcballCamera::OnWindowResizeEvent(Vec2i size) {
  screen_size_ = size;
}

void ArcballCamera::OnEventStatusChanged(bool) {}

glm::vec3 ArcballCamera::GetViewDir() const {
  return -glm::transpose(view_matrix_)[2];
}
glm::vec3 ArcballCamera::GetRightVector() const {
  return glm::transpose(view_matrix_)[0];
}
