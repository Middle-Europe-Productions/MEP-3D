#ifndef LAYER_CONTROLLER_HPP
#define LAYER_CONTROLLER_HPP

#include <MEP-3D/directional_light.hpp>
#include <MEP-3D/layer.hpp>
#include <MEP-3D/light_controller.hpp>
#include <MEP-3D/model.hpp>
#include <MEP-3D/observer.hpp>
#include <MEP-3D/observer_list.hpp>
#include <MEP-3D/shader.hpp>
#include <MEP-3D/spot_light.hpp>

#include <memory>
#include <vector>

class LayerController;
class LayerControllerObserver : public Observer {
 public:
  virtual void OnAttach(LayerController* scene) = 0;
  virtual void OnDetach() = 0;
  virtual ~LayerControllerObserver() = default;
};

class LayerController : public Layer,
                        public ObserverList<LayerControllerObserver> {
 public:
  enum UsableElements : int {
    DirectionalLights = 1 << 1,
    PointLights = 1 << 2,
    SpotLights = 1 << 3,
    Lights = DirectionalLights | PointLights | SpotLights,
  };
  enum DrawableElements : int { Models = 1 << 1, All = 1 << 2 };
  LayerController() = default;
  LayerController(const std::string& name) : Layer(name) {}
  void AddObserver(LayerControllerObserver* obs);
  std::vector<std::unique_ptr<DirectionalLight>>& GetDirectionaLights();
  std::size_t AttachDirectionaLight(
      std::unique_ptr<DirectionalLight> directional_light);
  void AttachSpotLightController(
      std::unique_ptr<SpotLightController> spot_light_controller);
  std::unique_ptr<SpotLightController>& GetSpotLightController();
  void AttachPointLightController(
      std::unique_ptr<PointLightController> point_light_controller);
  std::unique_ptr<PointLightController>& GetPointLightController();
  std::size_t AttachModel(std::unique_ptr<Model> model);
  std::vector<std::unique_ptr<Model>>& GetModels();
  std::size_t AttachShader(std::unique_ptr<Shader> shader);
  std::vector<std::unique_ptr<Shader>>& GetShader();
  void UseAllDirectionalLights();
  void UseAllPointLights();
  void UseAllSpotLights();
  void UseAllLights(UsableElements usable_elements = UsableElements::Lights);
  std::size_t DrawAllModels(RenderTarget& render_target);
  std::size_t DrawAll(
      RenderTarget& render_target,
      DrawableElements drawable_elements = DrawableElements::All);
  virtual ~LayerController();

 private:
  std::vector<std::unique_ptr<DirectionalLight>> directional_lights_;
  std::vector<std::unique_ptr<Model>> models_;
  std::vector<std::unique_ptr<Shader>> shaders_;
  std::unique_ptr<SpotLightController> spot_light_controller_;
  std::unique_ptr<PointLightController> point_light_controller_;
};

#endif