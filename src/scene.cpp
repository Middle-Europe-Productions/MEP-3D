#include <MEP-3D/scene.hpp>

namespace mep {
namespace {
template <typename ArrayT>
std::size_t DrawAllElements(ArrayT& elements, RenderTarget& render_target) {
  int draw_calls = 0;
  for (auto& element : elements) {
    if (element->ShouldDraw()) {
      element->Draw(render_target);
      draw_calls++;
    }
  }
  return draw_calls;
}
}  // namespace

void Scene::AddObserver(SceneObserver* obs) {
  if (obs) {
    ObserverList<SceneObserver>::AddObserver(obs);
    obs->OnSceneAttach(this);
  }
}
std::vector<std::unique_ptr<DirectionalLight>>& Scene::GetDirectionaLights() {
  return directional_lights_;
}

std::unique_ptr<SpotLightController>& Scene::GetSpotLightController() {
  return spot_light_controller_;
}

std::unique_ptr<PointLightController>& Scene::GetPointLightController() {
  return point_light_controller_;
}

std::vector<std::unique_ptr<Model>>& Scene::GetModels() {
  return models_;
}

std::vector<std::unique_ptr<Shader>>& Scene::GetShaders() {
  return shaders_;
}

std::vector<std::unique_ptr<Material>>& Scene::GetMaterial() {
  return materials_;
}

std::vector<std::unique_ptr<TextureBase>>& Scene::GetTexture() {
  return textures_;
}
std::vector<std::unique_ptr<CameraBase>>& Scene::GetCamera() {
  return camera_;
}

std::vector<std::unique_ptr<Volume>>& Scene::GetVolume() {
  return volumes_;
}

void Scene::Attach(std::unique_ptr<DirectionalLight> directional_light) {
  DCHECK(directional_light);
  directional_lights_.emplace_back(std::move(directional_light));
}

void Scene::Attach(std::unique_ptr<SpotLightController> spot_light_controller) {
  DCHECK(spot_light_controller);
  spot_light_controller_ = std::move(spot_light_controller);
}

void Scene::Attach(
    std::unique_ptr<PointLightController> point_light_controller) {
  DCHECK(point_light_controller);
  point_light_controller_ = std::move(point_light_controller);
}

void Scene::Attach(std::unique_ptr<Model> model) {
  DCHECK(model);
  models_.emplace_back(std::move(model));
  ForAllObservers([model = models_.back().get()](auto* obs) {
    obs->OnModelAttached(model);
  });
}

void Scene::Attach(std::unique_ptr<Shader> shader) {
  DCHECK(shader);
  shaders_.emplace_back(std::move(shader));
  ForAllObservers([shader = shaders_.back().get()](auto* obs) {
    obs->OnShaderAttached(shader);
  });
}

void Scene::Attach(std::unique_ptr<Material> material) {
  DCHECK(material);
  materials_.emplace_back(std::move(material));
  ForAllObservers([material = materials_.back().get()](auto* obs) {
    obs->OnMaterialAttached(material);
  });
}
void Scene::Attach(std::unique_ptr<TextureBase> texture) {
  DCHECK(texture);
  textures_.emplace_back(std::move(texture));
  ForAllObservers([texture = textures_.back().get()](auto* obs) {
    obs->OnTextureAttached(texture);
  });
}

void Scene::Attach(std::unique_ptr<Texture> texture) {
  DCHECK(texture);
  textures_.emplace_back(std::move(texture));
  ForAllObservers([texture = textures_.back().get()](auto* obs) {
    obs->OnTextureAttached(texture);
  });
}

void Scene::Attach(std::unique_ptr<Texture3D> texture) {
  DCHECK(texture);
  textures_.emplace_back(std::move(texture));
  ForAllObservers([texture = textures_.back().get()](auto* obs) {
    obs->OnTextureAttached(texture);
  });
}

void Scene::Attach(std::unique_ptr<CameraBase> camera_base) {
  DCHECK(camera_base);
  camera_.emplace_back(std::move(camera_base));
  ForAllObservers([camera_base = camera_.back().get()](auto* obs) {
    obs->OnCameraAttached(camera_base);
  });
}

void Scene::Attach(std::unique_ptr<PerspectiveCamera> camera_base) {
  Attach(std::unique_ptr<CameraBase>(std::move(camera_base)));
}

void Scene::Attach(std::unique_ptr<ArcballCamera> arcball_camera) {
  Attach(std::unique_ptr<CameraBase>(std::move(arcball_camera)));
}

void Scene::Attach(std::unique_ptr<Volume> volume) {
  DCHECK(volume);
  volumes_.emplace_back(std::move(volume));
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

std::size_t Scene::DrawAll(RenderTarget& render_target,
                           // TODO: Fix
                           DrawableElements drawable_elements) {
  std::size_t draw_calls = 0;
  if (drawable_elements == DrawableElements::Models ||
      drawable_elements == DrawableElements::All) {
    draw_calls += DrawAllElements(models_, render_target);
  }
  if (drawable_elements == DrawableElements::Volume ||
      drawable_elements == DrawableElements::All) {
    draw_calls += DrawAllElements(volumes_, render_target);
  }
  return draw_calls;
}

std::size_t Scene::UpdateCamera(float time_delta) {
  std::size_t update_counter = 0;
  for (auto& it : camera_) {
    it->Update(time_delta);
    ++update_counter;
  }
  return update_counter;
}

std::size_t Scene::UpdateAll(float time_delta,
                             UpdatableElements updatable_elements) {
  std::size_t update_counter = 0;
  if (updatable_elements == UpdatableElements::Camera ||
      updatable_elements == UpdatableElements::All) {
    update_counter += UpdateCamera(time_delta);
  }
  return update_counter;
}

Scene::~Scene() {
  LOG(INFO) << __FUNCTION__;
  ForAllObservers([](auto* obs) { obs->OnSceneDetach(); });
}
}  // namespace mep
