#include <MEP-3D/template/layer_assets_ui_layer.hpp>
#include <MEP-3D/template/util_common_draw.hpp>

#include <imgui.h>

class LayerAssetsUILayerImGUI : public LayerAssetsUILayer {
 public:
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void OnDraw(RenderTarget& render_target) override {
    if (!GetLayerAssetsPtr())
      return;
    ImGui::Begin("Scene", NULL, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("New")) {
        if (ImGui::BeginMenu("Light")) {
          if (ImGui::MenuItem("Spot Light")) {
            // TODO
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
    if (ImGui::CollapsingHeader("Directional Light")) {
      for (auto& dl_ptr : GetLayerAssetsPtr()->GetDirectionaLights()) {
        if (ImGui::TreeNode(dl_ptr->Identity::ToString().c_str())) {
          UI::DrawDirectionalLight(*dl_ptr.get());
          ImGui::Separator();
          ImGui::TreePop();
        }
      }
    }
    if (ImGui::CollapsingHeader("Spot Light")) {
      if (GetLayerAssetsPtr()->GetSpotLightController()) {
        for (auto& sl_ptr :
             GetLayerAssetsPtr()->GetSpotLightController()->GetContainer()) {
          if (ImGui::TreeNode(sl_ptr->Identity::ToString().c_str())) {
            UI::DrawSpotLight(*sl_ptr.get());
            ImGui::Separator();
            ImGui::TreePop();
          }
        }
      }
    }
    if (ImGui::CollapsingHeader("Point Light")) {
    }
    ImGui::End();
  }
};

std::unique_ptr<LayerAssetsUILayer> LayerAssetsUILayer::Create() {
  return std::make_unique<LayerAssetsUILayerImGUI>();
}
