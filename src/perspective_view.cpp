#include <MEP-3D/perspective_view.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mep {
PerspectiveView::PerspectiveView(PerspectiveView::Config config)
    : config_(config) {
  Update();
}

void PerspectiveView::UpdateConfig(PerspectiveView::Config config) {
  config_ = config;
  Update();
}

void PerspectiveView::UpdateAspectRation(GLfloat aspect_ration) {
  if (config_.aspect_ration != aspect_ration) {
    config_.aspect_ration = aspect_ration;
    Update();
  }
}

const glm::mat4& PerspectiveView::GetProjection() const {
  return projection_model_;
}

void PerspectiveView::Update() {
  projection_model_ =
      glm::perspective<GLfloat>(config_.field_of_view, config_.aspect_ration,
                                config_.near_ignore, config_.far_ignore);
}
}  // namespace mep
