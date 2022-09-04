#ifndef LAYER_CONTROLLER_UI_LAYER_HPP
#define LAYER_CONTROLLER_UI_LAYER_HPP

#include <MEP-3D/layer.hpp>
#include <MEP-3D/layer_controller.hpp>

class LayerControllerUILayer : public Layer, public LayerControllerObserver {
 public:
  void OnAttach(LayerController* scene);
  void OnDetach();
  LayerController* GetLayerControllerPtr();
  static std::unique_ptr<LayerControllerUILayer> Create();
  static std::unique_ptr<LayerControllerUILayer> Create(
      const std::string& config);
  virtual ~LayerControllerUILayer();

 protected:
  LayerController* scene_ptr_;
  LayerControllerUILayer();
};

#endif