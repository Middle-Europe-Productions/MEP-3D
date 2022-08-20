#include <MEP-3D/layer_controller.hpp>

void LayerController::AddObserver(LayerControllerObserver* obs) {
  if (obs) {
    ObserverList<LayerControllerObserver>::AddObserver(obs);
    obs->OnAttach(this);
  }
}
std::vector<std::unique_ptr<DirectionalLight>>&
LayerController::GetDirectionaLights() {
  return directional_lights_;
}
std::size_t LayerController::AttachDirectionaLight(
    std::unique_ptr<DirectionalLight> directional_light) {
  directional_lights_.emplace_back(std::move(directional_light));
  return directional_lights_.size() - 1;
}

void LayerController::AttachSpotLightController(
    std::unique_ptr<SpotLightController> spot_light_controller) {
  spot_light_controller_ = std::move(spot_light_controller);
}

std::unique_ptr<SpotLightController>&
LayerController::GetSpotLightController() {
  return spot_light_controller_;
}

void LayerController::AttachPointLightController(
    std::unique_ptr<PointLightController> point_light_controller) {
  point_light_controller_ = std::move(point_light_controller);
}

std::unique_ptr<PointLightController>&
LayerController::GetPointLightController() {
  return point_light_controller_;
}

void LayerController::UseAllDirectionalLights() {
  for (auto& dl : directional_lights_) {
    dl->Use();
  }
}

void LayerController::UseAllSpotLights() {
  spot_light_controller_->Use();
}

void LayerController::UseAllPointLights() {
  point_light_controller_->Use();
}

LayerController::~LayerController() {
  LOG(INFO) << __FUNCTION__;
  ForAllObservers([](auto* obs) { obs->OnDetach(); });
}