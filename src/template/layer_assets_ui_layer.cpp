#include <MEP-3D/common_names.hpp>
#include <MEP-3D/template/layer_assets_ui_layer.hpp>

void LayerAssetsUILayer::OnAttach(LayerAssets* scene) {
  LOG(INFO) << "Scene attached";
  scene_ptr_ = scene;
}

void LayerAssetsUILayer::OnDetach() {
  LOG(INFO) << "Scene detached";
  scene_ptr_ = nullptr;
}

LayerAssets* LayerAssetsUILayer::GetLayerAssetsPtr() {
  return scene_ptr_;
}

LayerAssetsUILayer::LayerAssetsUILayer() : Layer(kLayerAssetUILayer) {}

LayerAssetsUILayer::~LayerAssetsUILayer() {
  if (scene_ptr_) {
    scene_ptr_->RemoveObserver(this);
  }
}
