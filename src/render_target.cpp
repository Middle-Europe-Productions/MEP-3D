#include <MEP-3D/render_target.hpp>

RenderTarget::RenderTarget() : view_(nullptr), camera_(nullptr) {}

void RenderTarget::AddCamera(CameraBase* camera) {
  camera_ = camera;
}
void RenderTarget::RemoveCamera() {
  camera_ = nullptr;
}
CameraBase* RenderTarget::GetCamera() {
  return camera_;
}
const CameraBase* RenderTarget::GetCamera() const {
  return camera_;
}
void RenderTarget::AddView(ViewBase* view) {
  view_ = view;
}
void RenderTarget::RemoveView() {
  view_ = nullptr;
}
ViewBase* RenderTarget::GetView() {
  return view_;
}
const ViewBase* RenderTarget::GetView() const {
  return view_;
}
