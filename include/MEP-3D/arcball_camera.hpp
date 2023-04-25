#ifndef ARCBALL_CAMERA_HPP
#define ARCBALL_CAMERA_HPP

#include <MEP-3D/camera_base.hpp>
#include <glm/glm.hpp>

namespace mep {
enum class ArcballCameraActions : int {
  Left = 0,
  Right = 1,
  Up = 2,
  Down = 3,
  Grab,
  Forward,
  Backward,
  Count
};

std::string ToString(ArcballCameraActions pca);

using ArcballCameraControlsMap =
    std::array<Key, static_cast<int>(ArcballCameraActions::Count)>;

struct ArcballCameraConfig {
  glm::vec3 start_position;
  glm::vec3 start_up;
  glm::vec3 lookat_position;
  // Camera will observe the window after the initialization.
  Vec2i initial_viewport_size;
  float move_speed;
  static ArcballCameraConfig Create();
};

namespace UI {
class Drawer;
};

class ArcballCamera : public CameraBase {
 public:
  ArcballCamera(const ArcballCameraConfig& config,
                const ArcballCameraControlsMap& controls = {
                    Key::A, Key::D, Key::W, Key::S, Key::MouseLeft, Key::Q,
                    Key::Z});
  void Update(float) override;
  void OnKeyEvent(KeyEvent event) override;
  glm::mat4 GetViewMatrix() const override;
  void OnMouseEvent(MouseEvent event) override;
  void OnWindowResizeEvent(Vec2i size) override;
  void OnWindowPositionEvent(Vec2i pos) override;
  void OnEventStatusChanged(bool) override;
  float GetDistance() const;
  std::string ToString() const override;

 private:
  glm::vec3 GetViewDir() const;
  glm::vec3 GetRightVector() const;
  friend class UI::Drawer;

  glm::mat4 view_matrix_;
  glm::vec3 lookat_position_;
  glm::vec3 world_up_;
  Vec2f mouse_change_;
  Vec2i screen_size_;
  float move_speed_;
  bool initial_move_ = true;
  Vec2f last_mouse_position_ = {0.0, 0.0};
};
};  // namespace mep

#endif