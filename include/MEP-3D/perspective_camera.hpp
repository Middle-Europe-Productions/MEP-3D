#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <unordered_map>

#include <MEP-3D/camera_base.hpp>
#include <MEP-3D/key_event.hpp>
#include <MEP-3D/observable_map.hpp>
#include <MEP-3D/time_delta.hpp>
#include <MEP-3D/updatable.hpp>
#include <MEP-3D/window_observer.hpp>

#include <glog/logging.h>
#include <glm/gtx/string_cast.hpp>
#include <string>

enum class PerspectiveCameraActions : int {
  Front = 0,
  Back = 1,
  Left = 2,
  Right,
  Up,
  Down,
  Count
};

std::string ToString(PerspectiveCameraActions pca);

using PerspectiveCameraControlsMap =
    std::array<Key, static_cast<int>(PerspectiveCameraActions::Count)>;

struct PerspectiveCameraConfig {
  glm::vec3 start_position;
  glm::vec3 start_up;
  float start_yaw;
  float start_pitch;
  float start_move_speed;
  float start_turn_speed;
  static PerspectiveCameraConfig Create();
};

namespace UI {
class Drawer;
};

class PerspectiveCamera : public CameraBase {
 public:
  PerspectiveCamera(const PerspectiveCameraConfig& config,
                    const PerspectiveCameraControlsMap& controls = {
                        Key::W, Key::S, Key::A, Key::D, Key::Space,
                        Key::LeftControl});
  void Update(float time_delta) override;
  glm::mat4 GetViewMatrix() const override;
  void OnMouseEvent(MouseEvent event) override;
  void OnWindowResizeEvent(Vec2i size) override;
  void OnEventStatusChanged(bool events_blocked) override;
  std::string ToString() const override;

 private:
  friend class UI::Drawer;

  void ValidateKeyInput(float time_delta);

  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 world_up_;

  float yaw_;
  float pitch_;

  float move_speed_;
  float turn_speed_;

  float last_mouse_x_;
  float last_mouse_y_;
  float mouse_x_change;
  float mouse_y_change;
  bool initial_move_ = true;
  bool disabled_ = true;

  bool reversed_x_axis_ = false;
  bool reversed_y_axis_ = false;
};

#endif