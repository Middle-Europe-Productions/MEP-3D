#ifndef LAYER_ASSETS_HPP
#define LAYER_ASSETS_HPP

#include <MEP-3D/directional_light.hpp>
#include <MEP-3D/light_controller.hpp>
#include <MEP-3D/observer.hpp>
#include <MEP-3D/observer_list.hpp>
#include <MEP-3D/spot_light.hpp>

#include <memory>
#include <vector>

class LayerAssets;
class LayerAssetsObserver : public Observer {
 public:
  virtual void OnAttach(LayerAssets* scene) = 0;
  virtual void OnDetach() = 0;
  virtual ~LayerAssetsObserver() = default;
};

class LayerAssets : public ObserverList<LayerAssetsObserver> {
 public:
  LayerAssets() = default;
  void AddObserver(LayerAssetsObserver* obs);
  std::vector<std::unique_ptr<DirectionalLight>>& GetDirectionaLights();
  std::size_t AttachDirectionaLight(
      std::unique_ptr<DirectionalLight> directional_light);
  void AttachSpotLightController(
      std::unique_ptr<SpotLightController> spot_light_controller_);
  std::unique_ptr<SpotLightController>& GetSpotLightController();
  void AttachPointLightController(
      std::unique_ptr<PointLightController> point_light_controller);
  std::unique_ptr<PointLightController>& GetPointLightController();
  void UseAllDirectionalLights();

  virtual ~LayerAssets();

 private:
  std::vector<std::unique_ptr<DirectionalLight>> directional_lights_;
  std::unique_ptr<SpotLightController> spot_light_controller_;
  std::unique_ptr<PointLightController> point_light_controller_;
};

#endif