#include <MEP-3D/template/layer_assets_ui_layer.hpp>

#include <imgui.h>

namespace {
Color ToMepColor(ImVec4& im_color) {
  return Color(255 * im_color.x, 255 * im_color.y, 255 * im_color.z,
               255 * im_color.w);
}
ImVec4 FromMepColor(Color& color) {
  return ImVec4(color.Rf(), color.Gf(), color.Bf(), color.Af());
}
}  // namespace

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
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
    if (ImGui::CollapsingHeader("Directional Light")) {
      for (auto& dl_ptr : GetLayerAssetsPtr()->GetDirectionaLights()) {
        if (ImGui::TreeNode(dl_ptr->Identity::ToString().c_str())) {
          DrawDirectionalLight(dl_ptr.get());
          ImGui::Separator();
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
    ImGui::End();
  }
  void DrawDirectionalLight(DirectionalLight* directional_light) {
    if (ImGui::TreeNode("Ambient Config")) {
      DrawAmbientConfig(directional_light->GetAmbientConfigRef());
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Diffuse Config")) {
      DrawDiffuseConfig(directional_light->GetDiffuseConfigRef());
      ImGui::TreePop();
    }
  }
  void DrawAmbientConfig(AmbientConfig& config) {
    ImVec4 im_color = FromMepColor(config.color);
    ImGui::ColorEdit4("Light Color##2", (float*)&im_color,
                      ImGuiColorEditFlags_DisplayRGB);
    config.color = ToMepColor(im_color);
    ImGui::SliderFloat("Intensity", &config.intensity, 0.0f, 1.0f, "%.3f",
                       ImGuiSliderFlags_None);
  }
  void DrawDiffuseConfig(DiffuseConfig& config) {
    float v[3] = {config.direction.value().x_, config.direction.value().y_,
                  config.direction.value().z_};
    ImGui::DragFloat3("Position", v, 0.01f, -FLT_MAX, FLT_MAX);
    config.direction.value().x_ = v[0];
    config.direction.value().y_ = v[1];
    config.direction.value().z_ = v[2];
    config.direction.value();
    ImGui::SliderFloat("Intensity", &config.intensity, 0.0f, 1.0f, "%.3f",
                       ImGuiSliderFlags_None);
  }
};

std::unique_ptr<LayerAssetsUILayer> LayerAssetsUILayer::Create() {
  return std::make_unique<LayerAssetsUILayerImGUI>();
}
