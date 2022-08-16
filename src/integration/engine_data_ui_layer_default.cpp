#include <MEP-3D/template/engine_data_ui_layer.hpp>

class EngineDataUILayerDefault : public EngineDataUILayer {
  EngineDataUILayerDefault() : EngineDataUILayer() {}
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void OnDraw(RenderTarget& render_target) override {}
  void OnKeyEvent(KeyEvent event) override {}
  void OnMouseEvent(MouseEvent event) override {}
  void OnEventStatusChanged(bool events_blocked) override {}
  bool ShouldIgnoreLayer() const { return true; }
};

EngineDataUILayer::EngineDataUILayer() : Layer(__FUNCTION__) {}

std::unique_ptr<EngineDataUILayer> EngineDataUILayer::Create() {
  return std::make_unique<EngineDataUILayerDefault>();
}