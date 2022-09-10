#include <imgui.h>
#include <MEP-3D/common_names.hpp>
#include <MEP-3D/engine.hpp>
#include <MEP-3D/template/engine_data_ui_layer.hpp>
#include <MEP-3D/template/util_common_draw.hpp>

namespace {
constexpr Keyboard kBlockEventKey = Keyboard::B;
constexpr int kPlotElementsBuffer = 100;
}  // namespace

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
  void OnUpdate(float time_delta) override {
    if (!GetEngine())
      return;
    if (!block_applied_ && GetEngine()->GetWindow()) {
      GetEngine()->GetWindow()->BlockEvents(block_events_, Keyboard::B);
      block_applied_ = true;
    }
  }
  void OnDraw(RenderTarget& render_target) override {
    ImGui::Begin("Engine Data");
    ImGui::Checkbox("Pause", &should_not_update_);
    if (should_not_update_) {
      if (!buffer_applied) {
        buffer = GetEngine()->GetEngineMonitor();
        buffer_applied = true;
      }
      UI::DrawEngineMonitorDataConst(buffer);
    } else {
      const auto& engine_data = GetEngine()->GetEngineMonitor();
      UI::DrawEngineMonitorDataConst(engine_data);
      buffer_applied = false;
    }
    ImGui::End();
  }
  bool ShouldIgnoreLayer() const override { return false; }
  void OnKeyEvent(KeyEvent event) override {
    if (event.action == Action::Released && event.code == Keyboard::B) {
      LOG(INFO) << "Blocking events!";
      block_events_ = !block_events_;
      block_applied_ = false;
    }
  }
  void OnMouseEvent(MouseEvent event) override {}
  void OnEventStatusChanged(bool events_blocked) override {}

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
