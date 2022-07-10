#include<MEP-3D/three_dim_view.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/glm.hpp>

ThreeDimView::ThreeDimView(ThreeDimView::Config config) : config_(config) {
	Update();
}

void ThreeDimView::UpdateConfig(ThreeDimView::Config config) {
	config_ = config;
	Update();
}

const glm::mat4& ThreeDimView::GetProjection() const  {
	return projection_model_;
}

void ThreeDimView::Update() {
	projection_model_ =
		glm::perspective<GLfloat>(config_.field_of_view,
			config_.aspect_ration,
			config_.near_ignore,
			config_.far_ignore);
}