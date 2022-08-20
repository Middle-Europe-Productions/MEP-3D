#include <MEP-3D/common_names.hpp>
#include <MEP-3D/template/layer_controller_ui_layer.hpp>

void LayerControllerUILayer::OnAttach(LayerController* scene) {
  LOG(INFO) << "Scene attached";
  scene_ptr_ = scene;
}

void LayerControllerUILayer::OnDetach() {
  LOG(INFO) << "Scene detached";
  scene_ptr_ = nullptr;
}

LayerController* LayerControllerUILayer::GetLayerControllerPtr() {
  return scene_ptr_;
}

LayerControllerUILayer::LayerControllerUILayer()
    : Layer(kLayerControllerUILayer) {}

LayerControllerUILayer::~LayerControllerUILayer() {
  if (scene_ptr_) {
    scene_ptr_->RemoveObserver(this);
  }
}
