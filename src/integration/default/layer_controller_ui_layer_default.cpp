#include <MEP-3D/template/layer_controller_ui_layer.hpp>

class LayerControllerUILayerDefault : public LayerControllerUILayer {
 public:
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void OnDraw(RenderTarget& render_target) override {}

 private:
  virtual bool ShouldIgnoreLayer() const override { return true; }
};

std::unique_ptr<LayerControllerUILayer> LayerControllerUILayer::Create() {
  return std::make_unique<LayerControllerUILayerDefault>();
}

std::unique_ptr<LayerControllerUILayer> LayerControllerUILayer::Create(
    const std::string& config) {
  return std::make_unique<LayerControllerUILayerDefault>();
}
