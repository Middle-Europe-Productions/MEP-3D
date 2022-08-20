#ifndef LAYER_CONTROLLER_HPP
#define LAYER_CONTROLLER_HPP

#include <MEP-3D/directional_light.hpp>
#include <MEP-3D/layer.hpp>
#include <MEP-3D/light_controller.hpp>
#include <MEP-3D/observer.hpp>
#include <MEP-3D/observer_list.hpp>
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
  void UseAllDirectionalLights();
  void UseAllPointLights();
  void UseAllSpotLights();
  virtual ~LayerController();

 private:
  std::vector<std::unique_ptr<DirectionalLight>> directional_lights_;
  std::unique_ptr<SpotLightController> spot_light_controller_;
  std::unique_ptr<PointLightController> point_light_controller_;
};

#endif