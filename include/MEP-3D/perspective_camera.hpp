#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include <GL/glew.h>
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
#include <array>
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
    std::array<Keyboard, static_cast<int>(PerspectiveCameraActions::Count)>;

struct PerspectiveCameraConfig {
  glm::vec3 start_position;
  glm::vec3 start_up;
  GLfloat start_yaw;
  GLfloat start_pitch;
  GLfloat start_move_speed;
  GLfloat start_turn_speed;
  static PerspectiveCameraConfig Create();
};

namespace UI {
class Drawer;
};

class PerspectiveCamera : public CameraBase {
 public:
  PerspectiveCamera(const PerspectiveCameraConfig& config,
                    const PerspectiveCameraControlsMap& controls = {
                        Keyboard::W, Keyboard::S, Keyboard::A, Keyboard::D,
                        Keyboard::Space, Keyboard::LeftControl});
  void Update(float time_delta) override;
  glm::mat4 GetViewMatrix() const override;
  void OnKeyEvent(KeyEvent event) override;
  void OnMouseEvent(MouseEvent event) override;
  void OnWindowResizeEvent(Vec2i size) override;
  void OnEventStatusChanged(bool events_blocked) override;
  std::string ToString() const override;

 private:
  friend class UI::Drawer;

  bool IsActive(PerspectiveCameraActions pca);
  bool ContainsKey(Keyboard key);
  void InitKeyboardMap();
  void ValidateKeyboardInput(float time_delta);

  std::unordered_map<Keyboard, bool> key_status_;
  PerspectiveCameraControlsMap controls_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 world_up_;

  GLfloat yaw_;
  GLfloat pitch_;

  GLfloat move_speed_;
  GLfloat turn_speed_;

  GLfloat last_mouse_x_;
  GLfloat last_mouse_y_;
  GLfloat mouse_x_change;
  GLfloat mouse_y_change;
  bool initial_move_;
  bool disabled_ = true;

  bool reversed_x_axis_ = false;
  bool reversed_y_axis_ = false;
};

#endif