#include <MEP-3D/common_names.hpp>
#include <MEP-3D/engine.hpp>
#include <MEP-3D/template/engine_data_ui_layer.hpp>

namespace mep {
class EngineDataUILayerDefault : public EngineDataUILayer {
 public:
  EngineDataUILayerDefault() : EngineDataUILayer() {
    LOG(INFO) << __FUNCTION__;
  }
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void OnDraw(RenderTarget& render_target) override {}
  void OnKeyEvent(KeyEvent event) override {}
  void OnMouseEvent(MouseEvent event) override {}
  void OnWindowResizeEvent(Vec2i size) override {}
  void OnEventStatusChanged(bool events_blocked) override {}
  bool ShouldIgnoreLayer() const { return true; }
  virtual ~EngineDataUILayerDefault() {
    if (GetEngine() && GetEngine()->GetWindow())
      GetEngine()->GetWindow()->RemoveObserver(this);
  }
};

EngineDataUILayer::EngineDataUILayer() : Layer(kEngineDataUILayer) {}

std::unique_ptr<EngineDataUILayer> EngineDataUILayer::Create() {
  return std::make_unique<EngineDataUILayerDefault>();
}
}  // namespace mep
