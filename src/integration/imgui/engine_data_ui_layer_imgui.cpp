#include <imgui.h>
#include <MEP-3D/common_names.hpp>
#include <MEP-3D/engine.hpp>
#include <MEP-3D/template/engine_data_ui_layer.hpp>
#include <MEP-3D/template/util_common_draw.hpp>

namespace mep {
class EngineDataUILayerImGUI : public EngineDataUILayer {
  void OnAttach() override {
    auto engine = GetEngine();
    if (engine && engine->GetWindow()) {
      engine->GetWindow()->AddObserver(this);
    }
    block_events_ = true;
    block_applied_ = false;
  }
  void OnDetach() override {}
  void OnUpdate(float) override {
    if (!GetEngine())
      return;
    if (!block_applied_ && GetEngine()->GetWindow()) {
      GetEngine()->GetWindow()->BlockEvents(block_events_, Key::B);
      block_applied_ = true;
    }
  }
  void OnDraw(RenderTarget&) override {
    ImGui::Begin("Engine Data");
    ImGui::Checkbox("Pause", &should_not_update_);
    if (should_not_update_) {
      if (!buffer_applied) {
        buffer = GetEngine()->GetEngineMonitor();
        buffer_applied = true;
      }
      UI::Drawer::DrawEngineMonitorDataConst(buffer);
    } else {
      const auto& engine_data = GetEngine()->GetEngineMonitor();
      UI::Drawer::DrawEngineMonitorDataConst(engine_data);
      buffer_applied = false;
    }
    ImGui::End();
  }
  bool ShouldIgnoreLayer() const override { return false; }
  void OnKeyEvent(KeyEvent event) override {
    if (event.action == Action::Released && event.code == Key::B) {
      LOG(INFO) << "Blocking events!";
      block_events_ = !block_events_;
      block_applied_ = false;
    }
  }
  void OnMouseEvent(MouseEvent) override {}
  void OnWindowResizeEvent(Vec2i) override {}
  void OnWindowPositionEvent(Vec2i pos) override {}
  void OnEventStatusChanged(bool) override {}

 private:
  EngineMonitorData buffer;
  bool block_events_ = false;
  bool block_applied_ = true;
  bool should_not_update_ = false;
  bool buffer_applied = false;
};

EngineDataUILayer::EngineDataUILayer() : Layer(kEngineDataUILayer) {}

std::unique_ptr<EngineDataUILayer> EngineDataUILayer::Create() {
  return std::make_unique<EngineDataUILayerImGUI>();
}
}  // namespace mep
