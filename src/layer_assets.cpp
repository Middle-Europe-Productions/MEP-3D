#include <MEP-3D/layer_assets.hpp>

void LayerAssets::AddObserver(LayerAssetsObserver* obs) {
  if (obs) {
    ObserverList<LayerAssetsObserver>::AddObserver(obs);
    obs->OnAttach(this);
  }
}
std::vector<std::unique_ptr<DirectionalLight>>&
LayerAssets::GetDirectionaLights() {
  return directional_lights_;
}
std::size_t LayerAssets::AttachDirectionaLight(
    std::unique_ptr<DirectionalLight> directional_light) {
  directional_lights_.emplace_back(std::move(directional_light));
  return directional_lights_.size() - 1;
}

void LayerAssets::AttachSpotLightController(
    std::unique_ptr<SpotLightController> spot_light_controller_) {
  spot_light_controller_ = std::move(spot_light_controller_);
}

std::unique_ptr<SpotLightController>& LayerAssets::GetSpotLightController() {
  return spot_light_controller_;
}

void LayerAssets::AttachPointLightController(
    std::unique_ptr<PointLightController> point_light_controller) {
  point_light_controller_ = std::move(point_light_controller);
}

std::unique_ptr<PointLightController>& LayerAssets::GetPointLightController() {
  return point_light_controller_;
}

void LayerAssets::UseAllDirectionalLights() {
  for (auto& dl : directional_lights_) {
    dl->Use();
  }
}

LayerAssets::~LayerAssets() {
  LOG(INFO) << __FUNCTION__;
  ForAllObservers([](auto* obs) { obs->OnDetach(); });
}