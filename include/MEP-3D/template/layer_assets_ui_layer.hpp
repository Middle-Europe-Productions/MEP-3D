#ifndef LAYER_ASSETS_UI_LAYER_HPP
#define LAYER_ASSETS_UI_LAYER_HPP

#include <MEP-3D/layer.hpp>
#include <MEP-3D/layer_assets.hpp>

class LayerAssetsUILayer : public Layer, public LayerAssetsObserver {
 public:
  void OnAttach(LayerAssets* scene);
  void OnDetach();
  LayerAssets* GetLayerAssetsPtr();
  static std::unique_ptr<LayerAssetsUILayer> Create();
  virtual ~LayerAssetsUILayer();

 protected:
  LayerAssets* scene_ptr_;
  LayerAssetsUILayer();
};

#endif