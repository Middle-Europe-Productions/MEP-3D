#ifndef THREE_DIM_VIEW_HPP
#define THREE_DIM_VIEW_HPP

#include <GL/glew.h>
#include <MEP-3D/view_base.hpp>
#include <functional>
#include <glm/glm.hpp>

class PerspectiveView : public ViewBase {
 public:
  struct Config {
    GLfloat field_of_view;
    GLfloat aspect_ration;
    GLfloat near_ignore;
    GLfloat far_ignore;
  };
  PerspectiveView(Config config);
  void UpdateConfig(Config config);
  const glm::mat4& GetProjection() const override;

 private:
  void Update();
  Config config_;
  glm::mat4 projection_model_;
};

#endif