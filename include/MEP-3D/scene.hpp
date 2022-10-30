#ifndef SCENE_HPP
#define SCENE_HPP

#include <MEP-3D/directional_light.hpp>
#include <MEP-3D/layer.hpp>
#include <MEP-3D/light_controller.hpp>
#include <MEP-3D/material.hpp>
#include <MEP-3D/model.hpp>
#include <MEP-3D/observer.hpp>
#include <MEP-3D/observer_list.hpp>
#include <MEP-3D/perspective_camera.hpp>
#include <MEP-3D/shader.hpp>
#include <MEP-3D/spot_light.hpp>

#include <memory>
#include <vector>

class Scene;
class SceneObserver : public Observer {
 public:
  virtual void OnAttach(Scene* scene) = 0;
  virtual void OnDetach() = 0;
  virtual void OnModelAttached(Model* model) { DCHECK(model); };
  virtual void OnShaderAttached(Shader* shader) { DCHECK(shader); };
  virtual void OnMaterialAttached(Material* material) { DCHECK(material); };
  virtual void OnTextureAttached(Texture* texture) { DCHECK(texture); };
  virtual void OnPerspectiveCameraAttached(
      PerspectiveCamera* perspective_camera) {
    DCHECK(perspective_camera);
  };
  virtual ~SceneObserver() = default;
};

class Scene : public Layer, public ObserverList<SceneObserver> {
 public:
  enum class UsableElements : int {
    DirectionalLights = 1,
    PointLights = 2,
    SpotLights = 3,
    All
  };
  enum class UpdatableElements : int { Camera = 1, All };
  enum class DrawableElements : int { Models = 1, All };
  Scene() = default;
  Scene(const std::string& name) : Layer(name) {}
  void AddObserver(SceneObserver* obs);
  // Attachers and getters
  std::vector<std::unique_ptr<DirectionalLight>>& GetDirectionaLights();
  std::size_t Attach(std::unique_ptr<DirectionalLight> directional_light);
  void Attach(std::unique_ptr<SpotLightController> spot_light_controller);
  std::unique_ptr<SpotLightController>& GetSpotLightController();
  void Attach(std::unique_ptr<PointLightController> point_light_controller);
  std::unique_ptr<PointLightController>& GetPointLightController();
  std::size_t Attach(std::unique_ptr<Model> model);
  std::vector<std::unique_ptr<Model>>& GetModels();
  std::size_t Attach(std::unique_ptr<Shader> shader);
  std::vector<std::unique_ptr<Shader>>& GetShaders();
  std::size_t Attach(std::unique_ptr<Material> material);
  std::vector<std::unique_ptr<Material>>& GetMaterial();
  std::size_t Attach(std::unique_ptr<Texture> texture);
  std::vector<std::unique_ptr<Texture>>& GetTexture();
  std::size_t Attach(std::unique_ptr<PerspectiveCamera> perspective_camera);
  std::vector<std::unique_ptr<PerspectiveCamera>>& GetPerspectiveCamera();
  template <typename First, typename... Args>
  void Attach(First&& first, Args&&... args);
  // Usable
  void UseAllDirectionalLights();
  void UseAllPointLights();
  void UseAllSpotLights();
  void UseAllLights(UsableElements usable_elements = UsableElements::All);
  // Drawable
  std::size_t DrawAllModels(RenderTarget& render_target);
  std::size_t DrawAll(
      RenderTarget& render_target,
      DrawableElements drawable_elements = DrawableElements::All);
  // Updatable
  std::size_t UpdatePerspectiveCamera(float time_delta);
  std::size_t UpdateAll(
      float time_delta,
      UpdatableElements updatable_elements = UpdatableElements::All);
  // Helpers
  CameraBase* GetMasterCamera();
  virtual ~Scene();

 private:
  std::vector<std::unique_ptr<DirectionalLight>> directional_lights_;
  std::vector<std::unique_ptr<Model>> models_;
  std::vector<std::unique_ptr<Shader>> shaders_;
  std::vector<std::unique_ptr<Material>> materials_;
  std::vector<std::unique_ptr<Texture>> textures_;
  std::vector<std::unique_ptr<PerspectiveCamera>> perspective_camera_;
  std::unique_ptr<SpotLightController> spot_light_controller_;
  std::unique_ptr<PointLightController> point_light_controller_;
  // This might not work
  CameraBase* master_camera_ = nullptr;
};

template <typename First, typename... Args>
void Scene::Attach(First&& first, Args&&... args) {
  Attach(std::forward<First>(first));
  Attach(std::forward<Args>(args)...);
}

#endif