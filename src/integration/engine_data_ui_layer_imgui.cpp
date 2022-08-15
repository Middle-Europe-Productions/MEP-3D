#include <imgui.h>
#include <MEP-3D/engine.hpp>
#include <MEP-3D/template/engine_data_ui_layer.hpp>

EngineDataUILayer::EngineDataUILayer() : Layer(__FUNCTION__) {}
void EngineDataUILayer::OnAttach() {}
void EngineDataUILayer::OnDetach() {}
void EngineDataUILayer::OnUpdate(float time_delta) {}
void EngineDataUILayer::OnDraw(RenderTarget& render_target) {
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

bool EngineDataUILayer::ShouldIgnoreLayer() const {
  return false;
}
