#include <MEP-3D/scene.hpp>

void Scene::AddObserver(SceneObserver* obs) {
  if (obs) {
    ObserverList<SceneObserver>::AddObserver(obs);
    obs->OnAttach(this);
  }
}
std::vector<std::unique_ptr<DirectionalLight>>& Scene::GetDirectionaLights() {
  return directional_lights_;
}
std::size_t Scene::Attach(std::unique_ptr<DirectionalLight> directional_light) {
  DCHECK(directional_light);
  directional_lights_.emplace_back(std::move(directional_light));
  return directional_lights_.size() - 1;
}

void Scene::Attach(std::unique_ptr<SpotLightController> spot_light_controller) {
  DCHECK(spot_light_controller);
  spot_light_controller_ = std::move(spot_light_controller);
}

std::unique_ptr<SpotLightController>& Scene::GetSpotLightController() {
  return spot_light_controller_;
}

void Scene::Attach(
    std::unique_ptr<PointLightController> point_light_controller) {
  DCHECK(point_light_controller);
  point_light_controller_ = std::move(point_light_controller);
}

std::unique_ptr<PointLightController>& Scene::GetPointLightController() {
  return point_light_controller_;
}

std::size_t Scene::Attach(std::unique_ptr<Model> model) {
  DCHECK(model);
  models_.emplace_back(std::move(model));
  ForAllObservers([model = models_.back().get()](auto* obs) {
    obs->OnModelAttached(model);
  });
  return models_.size() - 1;
}

std::vector<std::unique_ptr<Model>>& Scene::GetModels() {
  return models_;
}

std::size_t Scene::Attach(std::unique_ptr<Shader> shader) {
  DCHECK(shader);
  shaders_.emplace_back(std::move(shader));
  ForAllObservers([shader = shaders_.back().get()](auto* obs) {
    obs->OnShaderAttached(shader);
  });
  return shaders_.size() - 1;
}

std::vector<std::unique_ptr<Shader>>& Scene::GetShaders() {
  return shaders_;
}

std::size_t Scene::Attach(std::unique_ptr<Material> material) {
  DCHECK(material);
  materials_.emplace_back(std::move(material));
  ForAllObservers([material = materials_.back().get()](auto* obs) {
    obs->OnMaterialAttached(material);
  });
  return materials_.size() - 1;
}
std::vector<std::unique_ptr<Material>>& Scene::GetMaterial() {
  return materials_;
}

std::size_t Scene::Attach(std::unique_ptr<Texture> texture) {
  DCHECK(texture);
  textures_.emplace_back(std::move(texture));
  ForAllObservers([texture = textures_.back().get()](auto* obs) {
    obs->OnTextureAttached(texture);
  });
  return textures_.size() - 1;
}

std::vector<std::unique_ptr<Texture>>& Scene::GetTexture() {
  return textures_;
}

std::size_t Scene::Attach(
    std::unique_ptr<PerspectiveCamera> perspective_camera) {
  DCHECK(perspective_camera);
  if (!master_camera_) {
    LOG(INFO) << "Updating master camera to " << perspective_camera->ToString();
    master_camera_ = perspective_camera.get();
  }
  perspective_camera_.emplace_back(std::move(perspective_camera));
  ForAllObservers(
      [perspective_camera = perspective_camera_.back().get()](auto* obs) {
        obs->OnPerspectiveCameraAttached(perspective_camera);
      });
  return perspective_camera_.size() - 1;
}

std::vector<std::unique_ptr<PerspectiveCamera>>& Scene::GetPerspectiveCamera() {
  return perspective_camera_;
}

void Scene::UseAllDirectionalLights() {
  for (auto& dl : directional_lights_) {
    dl->Use();
  }
}

void Scene::UseAllSpotLights() {
  spot_light_controller_->Use();
}

void Scene::UseAllPointLights() {
  point_light_controller_->Use();
}

void Scene::UseAllLights(UsableElements usable_elements) {
  if (usable_elements == UsableElements::DirectionalLights ||
      usable_elements == UsableElements::All)
    UseAllDirectionalLights();
  if (usable_elements == UsableElements::SpotLights ||
      usable_elements == UsableElements::All)
    UseAllSpotLights();
  if (usable_elements == UsableElements::PointLights ||
      usable_elements == UsableElements::All)
    UseAllPointLights();
}

std::size_t Scene::DrawAllModels(RenderTarget& render_target) {
  int draw_calls = 0;
  for (auto& model : models_) {
    if (model->ShouldDraw()) {
      model->Draw(render_target);
      draw_calls++;
    }
  }
  return draw_calls;
}

std::size_t Scene::DrawAll(RenderTarget& render_target,
                           // TODO: Fix
                           DrawableElements drawable_elements) {
  std::size_t draw_calls = 0;
  if (drawable_elements == DrawableElements::Models ||
      drawable_elements == DrawableElements::All) {
    draw_calls += DrawAllModels(render_target);
  }
  return draw_calls;
}

std::size_t Scene::UpdatePerspectiveCamera(float time_delta) {
  std::size_t update_counter = 0;
  for (auto& it : perspective_camera_) {
    it->Update();
    ++update_counter;
  }
  return update_counter;
}

std::size_t Scene::UpdateAll(float time_delta,
                             UpdatableElements updatable_elements) {
  std::size_t update_counter = 0;
  if (updatable_elements == UpdatableElements::Camera ||
      updatable_elements == UpdatableElements::All) {
    update_counter += UpdatePerspectiveCamera(time_delta);
  }
  return update_counter;
}

CameraBase* Scene::GetMasterCamera() {
  return master_camera_;
}

Scene::~Scene() {
  LOG(INFO) << __FUNCTION__;
  ForAllObservers([](auto* obs) { obs->OnDetach(); });
}