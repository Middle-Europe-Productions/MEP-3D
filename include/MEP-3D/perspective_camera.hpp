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
#include <glm/gtx/string_cast.hpp>
#include <string>

struct PerspectiveCameraControls {
  PerspectiveCameraControls()
      : up(Keyboard::W),
        down(Keyboard::S),
        left(Keyboard::A),
        right(Keyboard::D) {}
  Keyboard up;
  Keyboard down;
  Keyboard left;
  Keyboard right;
};

struct CameraConfig {
  glm::vec3 start_position;
  glm::vec3 start_up;
  GLfloat start_yaw;
  GLfloat start_pitch;
  GLfloat start_move_speed;
  GLfloat start_turn_speed;
};

class PerspectiveCamera : public CameraBase {
 public:
  PerspectiveCamera(
      const CameraConfig& config,
      const PerspectiveCameraControls& controls = PerspectiveCameraControls());
  void Update() override;
  glm::mat4 GetViewMatrix() const override;
  glm::vec3 GetPosition() const override;
  glm::vec3 GetNormalizedDirection() const override;
  void OnKeyEvent(KeyEvent event) override;
  void OnMouseEvent(MouseEvent event) override;
  void OnWindowResizeEvent(Vec2i size) override;
  void OnEventStatusChanged(bool events_blocked) override;
  std::string ToString() const;

 private:
  void InitKeyboardMap();
  void ValidateKeyboardInput();

  std::unordered_map<Keyboard, bool> key_status_;
  PerspectiveCameraControls controls_;
  TimeDeltaPtr camera_time_delta_;
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
};

#endif