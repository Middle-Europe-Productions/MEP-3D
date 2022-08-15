#ifndef LAYER_ASSETS_HPP
#define LAYER_ASSETS_HPP

#include <MEP-3D/directional_light.hpp>
#include <MEP-3D/observer_list.hpp>
#include <MEP-3D/spot_light.hpp>
#include <vector>

class LayerAssets;
class LayerAssetsObserver {
 public:
  virtual void OnAttach(LayerAssets* scene) = 0;
  virtual void OnDetach() = 0;
};

class LayerAssets : ObserverList<LayerAssetsObserver> {
 public:
  LayerAssets() = default;
  void AddObserver(LayerAssetsObserver* obs);
  std::vector<std::unique_ptr<DirectionalLight>>& GetDirectionaLights();
  std::size_t AttachDirectionaLight(
      std::unique_ptr<DirectionalLight> directional_light);
  std::vector<std::unique_ptr<SpotLight>>& GetSpotLights();
  void UseAllDirectionalLights();

  virtual ~LayerAssets();

 private:
  std::vector<std::unique_ptr<DirectionalLight>> directional_lights_;
  std::vector<std::unique_ptr<SpotLight>> spot_lights_;
};

#endif