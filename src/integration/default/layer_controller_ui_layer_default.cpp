#include <MEP-3D/template/layer_controller_ui_layer.hpp>

class LayerControllerUILayerDefault : public LayerControllerUILayer {
 public:
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void OnDraw(RenderTarget& render_target) override {}
};

std::unique_ptr<LayerControllerUILayer> LayerControllerUILayer::Create() {
  return std::make_unique<LayerControllerUILayerDefault>();
}
