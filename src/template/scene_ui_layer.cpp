#include <MEP-3D/common_names.hpp>
#include <MEP-3D/template/scene_ui_layer.hpp>

namespace mep {
void SceneUILayer::OnSceneAttach(Scene* scene) {
  LOG(INFO) << "Scene attached";
  scene_ptr_ = scene;
}

void SceneUILayer::OnSceneDetach() {
  LOG(INFO) << "Scene detached";
  scene_ptr_ = nullptr;
}

Scene* SceneUILayer::GetScenePtr() {
  return scene_ptr_;
}

SceneUILayer::SceneUILayer() : Layer(kSceneUILayer) {}

SceneUILayer::~SceneUILayer() {
  if (scene_ptr_) {
    scene_ptr_->RemoveObserver(this);
  }
}
}  // namespace mep
