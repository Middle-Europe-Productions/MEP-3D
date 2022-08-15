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

std::vector<std::unique_ptr<SpotLight>>& LayerAssets::GetSpotLights() {
  return spot_lights_;
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