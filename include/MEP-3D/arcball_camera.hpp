#ifndef ARCBALL_CAMERA_HPP
#define ARCBALL_CAMERA_HPP

#include <GL/glew.h>
#include <MEP-3D/camera_base.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct ArcballCameraConfig {
  glm::vec3 start_position;
  glm::vec3 start_up;
  glm::vec3 lookat_position;
  GLfloat start_move_speed;
  static ArcballCameraConfig Create();
};

namespace UI {
class Drawer;
};

class ArcballCamera : public CameraBase {
public:
  ArcballCamera(const ArcballCameraConfig &config)
      : CameraBase(kArcballCamera) {
    Set(CameraVariables::Position, config.start_position);
    lookat_position_ = config.lookat_position;
    world_up_ = config.start_up;
  }
  void Update(float time_delta) override {}
  glm::mat4 GetViewMatrix() const override {
    return glm::lookAt(Get(CameraVariables::Position), lookat_position_,world_up_);
  }
  void OnKeyEvent(KeyEvent event) override {}
  void OnMouseEvent(MouseEvent event) override {}
  void OnWindowResizeEvent(Vec2i size) override {}
  void OnEventStatusChanged(bool events_blocked) override {}

private:
  friend class UI::Drawer;

  glm::vec3 lookat_position_;
  glm::vec3 world_up_;
};

#endif