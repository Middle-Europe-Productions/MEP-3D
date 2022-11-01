#ifndef ARCBALL_CAMERA_HPP
#define ARCBALL_CAMERA_HPP

#include <GL/glew.h>
#include <MEP-3D/camera_base.hpp>

struct ArcballCameraConfig {
  glm::vec3 start_position;
  glm::vec3 start_up;
  GLfloat start_yaw;
  GLfloat start_pitch;
  GLfloat start_move_speed;
};

namespace UI {
class Drawer;
};

class ArcballCamera : public CameraBase {
 public:
  ArcballCamera(const ArcballCameraConfig& config);
  void Update(float time_delta) override {}
  glm::mat4 GetViewMatrix() const override {}
  void OnKeyEvent(KeyEvent event) override {}
  void OnMouseEvent(MouseEvent event) override {}
  void OnWindowResizeEvent(Vec2i size) override {}
  void OnEventStatusChanged(bool events_blocked) override {}

 private:
  friend class UI::Drawer;
};

#endif