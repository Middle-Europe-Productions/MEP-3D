#include <MEP-3D/template/util_common_draw.hpp>

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

namespace UI {
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
void DrawDirectionalLight(DirectionalLight& directional_light) {
  if (ImGui::TreeNode("Ambient Config")) {
    UI::DrawAmbientConfig(directional_light.GetAmbientConfigRef());
    ImGui::TreePop();
  }
  if (ImGui::TreeNode("Diffuse Config")) {
    UI::DrawDiffuseConfig(directional_light.GetDiffuseConfigRef());
    ImGui::TreePop();
  }
}
void DrawSpotConfig(SpotConfig& point_config) {}
void DrawSpotLight(SpotLight& point_light) {}
void DrawPointConfig(PointConfig& point_config) {}
void DrawPointLight(PointLight& point_light) {}
}  // namespace UI