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

std::size_t LayerController::AttachModel(std::unique_ptr<Model> model) {
  models_.emplace_back(std::move(model));
  return models_.size() - 1;
}

std::vector<std::unique_ptr<Model>>& LayerController::GetModels() {
  return models_;
}

std::size_t LayerController::AttachShader(std::unique_ptr<Shader> shader) {
  shaders_.emplace_back(std::move(shader));
  return shaders_.size() - 1;
}

std::vector<std::unique_ptr<Shader>>& LayerController::GetShader() {
  return shaders_;
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

void LayerController::UseAllLights(UsableElements usable_elements) {
  if (usable_elements & UsableElements::DirectionalLights)
    UseAllDirectionalLights();
  if (usable_elements & UsableElements::SpotLights)
    UseAllSpotLights();
  if (usable_elements & UsableElements::PointLights)
    UseAllPointLights();
}

std::size_t LayerController::DrawAllModels(RenderTarget& render_target) {
  for (auto& model : models_) {
    model->Draw(render_target);
  }
  return models_.size();
}

std::size_t LayerController::DrawAll(RenderTarget& render_target,
                                     // TODO: Fix
                                     DrawableElements drawable_elements) {
  std::size_t draw_calls = 0;
  if (drawable_elements & DrawableElements::Models) {
    draw_calls += DrawAllModels(render_target);
  }
  return draw_calls;
}

LayerController::~LayerController() {
  LOG(INFO) << __FUNCTION__;
  ForAllObservers([](auto* obs) { obs->OnDetach(); });
}