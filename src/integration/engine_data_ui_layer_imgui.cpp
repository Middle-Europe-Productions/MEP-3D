#include <imgui.h>
#include <MEP-3D/engine.hpp>
#include <MEP-3D/template/engine_data_ui_layer.hpp>

namespace {
constexpr Keyboard kBlockEventKey = Keyboard::B;
}

class EngineDataUILayerImGUI : public EngineDataUILayer {
  void OnAttach() override {
    auto engine = GetEngine();
    if (engine && engine->GetWindow()) {
      engine->GetWindow()->AddObserver(this);
    }
  }
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {
    if (GetEngine() && GetEngine()->GetWindow()) {
      GetEngine()->GetWindow()->BlockEvents(block_events_, Keyboard::B);
    }
  }
  void OnDraw(RenderTarget& render_target) override {
    const auto& engine_data = GetEngine()->GetEngineMonitor();
    ImGui::Begin("Engine Data");
    ImGui::Text("Frame rate: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Total frame time: %.1f", engine_data.frame_data.frame_time);
    if (ImGui::TreeNode("Layers")) {
      for (auto& ld : engine_data.frame_data.layer_data) {
        if (ImGui::TreeNode(ld.layer_name.c_str())) {
          ImGui::Text(ld.identity.ToString().c_str());
          ImGui::Text("Layer draw time: %.3f", ld.layer_draw_time_ms);
          ImGui::Text("Layer update time: %.3f", ld.layer_update_time_ms);
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
    ImGui::Separator();
    for (auto& sd : engine_data.frame_data.structure_data) {
      if (ImGui::TreeNode(sd.structure_name.c_str())) {
        for (auto& ld : sd.layer_array) {
          if (ImGui::TreeNode(ld.layer_name.c_str())) {
            ImGui::Text(ld.identity.ToString().c_str());
            ImGui::Text("Layer draw time: %.3f", ld.layer_draw_time_ms);
            ImGui::Text("Layer update time: %.3f", ld.layer_update_time_ms);
            ImGui::TreePop();
          }
        }
        ImGui::TreePop();
      }
    }
    ImGui::End();
  }

  bool ShouldIgnoreLayer() const { return false; }
  void OnKeyEvent(KeyEvent event) override {
    if (event.action == Action::Released && event.code == Keyboard::B) {
      LOG(INFO) << "Blocking events!";
      block_events_ = !block_events_;
    }
  }
  void OnMouseEvent(MouseEvent event) override {}
  void OnEventStatusChanged(bool events_blocked) override {}

 private:
  bool block_events_ = false;
};

EngineDataUILayer::EngineDataUILayer() : Layer(__FUNCTION__) {}

std::unique_ptr<EngineDataUILayer> EngineDataUILayer::Create() {
  return std::make_unique<EngineDataUILayerImGUI>();
}
