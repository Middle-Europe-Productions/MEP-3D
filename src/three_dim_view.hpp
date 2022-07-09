#ifndef THREE_DIM_VIEW_HPP
#define THREE_DIM_VIEW_HPP

#include "view_base.hpp"
#include <glm/glm.hpp>
#include <functional>

class ThreeDimView : public ViewBase {
public:
	struct Config {
		GLfloat field_of_view;
		GLfloat aspect_ration;
		GLfloat near_ignore;
		GLfloat far_ignore;
	};
	ThreeDimView(Config config): config_(config) {
		Update();
	}

	void UpdateConfig(Config config) {
		config_ = config;
		Update();
	}

	const glm::mat4& GetProjection() const override {
		return projection_model_;
	}
private:
	void Update() {
		projection_model_ =
			glm::perspective<GLfloat>(config_.field_of_view,
				config_.aspect_ration,
				config_.near_ignore,
				config_.far_ignore);
	}
	Config config_;
	glm::mat4 projection_model_;
};

#endif