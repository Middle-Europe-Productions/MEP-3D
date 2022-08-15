#include <MEP-3D/template/layer_assets_ui_layer.hpp>

class LayerAssetsUILayerDefault : public LayerAssetsUILayer {
 public:
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void OnDraw(RenderTarget& render_target) override {}
};

std::unique_ptr<LayerAssetsUILayer> LayerAssetsUILayer::Create() {
  return std::make_unique<LayerAssetsUILayerDefault>();
}
