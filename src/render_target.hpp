#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include<memory>

#include "view_base.hpp"
#include "camera.hpp"

class RenderTarget {
public:
	void AddCamera(Camera* camera) {
		camera_ = camera;
	}
	void RemoveCamera() {
		camera_ = nullptr;
	}
	Camera* GetCamera() {
		return camera_;
	}
	const Camera* GetCamera() const {
		return camera_;
	}
	void AddView(ViewBase* view) {
		view_ = view;
	}
	void RemoveView() {
		view_ = nullptr;
	}
	ViewBase* GetView() {
		return view_;
	}
	const ViewBase* GetView() const {
		return view_;
	}
	virtual ~RenderTarget() = default;
private:
	ViewBase* view_;
	Camera* camera_;
};

#endif