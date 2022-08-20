#include <MEP-3D/template/util_common_draw.hpp>

#include <imgui.h>
#include <numeric>
namespace {
Color ToMepColor(ImVec4& im_color) {
  return Color(255 * im_color.x, 255 * im_color.y, 255 * im_color.z,
               255 * im_color.w);
}
ImVec4 FromMepColor(Color& color) {
  return ImVec4(color.Rf(), color.Gf(), color.Bf(), color.Af());
}
constexpr int kGlobalPlotBufferSize = 100;
// Framerate
static std::vector<float> framerate_cache = std::vector<float>();
static float framerate_average = 0.0f;
static float framerate_offset = 10.0f;
static int frame = 0;
// Deg
constexpr int kDegMin = 1;
constexpr int kDegMax = 180;
}  // namespace

namespace UI {
void DrawAmbientConfig(AmbientConfig& config) {
  ImVec4 im_color = FromMepColor(config.color);
  ImGui::ColorEdit4("Light Color##2", (float*)&im_color,
                    ImGuiColorEditFlags_DisplayRGB);
  config.color = ToMepColor(im_color);
  ImGui::SliderFloat("Ambient Intensity", &config.intensity, 0.0f, 1.0f, "%.3f",
                     ImGuiSliderFlags_None);
}
void DrawDiffuseConfig(DiffuseConfig& config) {
  if (config.direction.has_value()) {
    float v[3] = {config.direction.value().x_, config.direction.value().y_,
                  config.direction.value().z_};
    ImGui::DragFloat3("Position", v, 0.05f, -FLT_MAX, FLT_MAX);
    config.direction.value().x_ = v[0];
    config.direction.value().y_ = v[1];
    config.direction.value().z_ = v[2];
  }
  ImGui::SliderFloat("Diffuse Intensity", &config.intensity, 0.0f, 1.0f, "%.3f",
                     ImGuiSliderFlags_None);
}
void DrawDirectionalLight(DirectionalLight& directional_light) {
  ImGui::Text("Ambient Config");
  UI::DrawAmbientConfig(directional_light.GetAmbientConfigRef());
  ImGui::Text("Diffuse Config");
  UI::DrawDiffuseConfig(directional_light.GetDiffuseConfigRef());
}
void DrawSpotConfig(SpotConfig& point_config) {
  int deg = static_cast<int>(point_config.edge_deg);
  ImGui::DragScalar("Degree", ImGuiDataType_S32, &deg, 1.0, &kDegMin, &kDegMax,
                    "%i deg");
  point_config.edge_deg = static_cast<float>(deg);
  float v[3] = {point_config.direction.x_, point_config.direction.y_,
                point_config.direction.z_};
  ImGui::SliderFloat3("Direction", v, -1.0, 1.0);
  point_config.direction.x_ = v[0];
  point_config.direction.y_ = v[1];
  point_config.direction.z_ = v[2];
}
void DrawSpotLight(SpotLight& point_light) {
  ImGui::Text("Ambient Config");
  UI::DrawAmbientConfig(point_light.GetAmbientConfigRef());
  ImGui::Text("Point Config");
  UI::DrawPointConfig(point_light.GetPointConfigRef());
  ImGui::Text("Diffuse Config");
  UI::DrawDiffuseConfig(point_light.GetDiffuseConfigRef());
  ImGui::Text("Spot Config");
  UI::DrawSpotConfig(point_light.GetSpotConfigRef());
}
void DrawPointConfig(PointConfig& point_config) {
  float vl = 10.0;
  ImGui::DragFloat("Quadratic", &point_config.quadratic, 0.01f, 0.0f, FLT_MAX,
                   "%.3f", ImGuiSliderFlags_None);
  ImGui::DragFloat("Linear", &point_config.linear, 0.01f, 0.0f, FLT_MAX, "%.3f",
                   ImGuiSliderFlags_None);
  ImGui::DragFloat("Constant", &point_config.constant, 0.01f, 0.0f, FLT_MAX,
                   "%.3f", ImGuiSliderFlags_None);
  float v[3] = {point_config.position.x_, point_config.position.y_,
                point_config.position.z_};
  ImGui::DragFloat3("Position", v, 0.05f, -FLT_MAX, FLT_MAX);
  point_config.position.x_ = v[0];
  point_config.position.y_ = v[1];
  point_config.position.z_ = v[2];
}
void DrawPointLight(PointLight& point_light) {
  ImGui::Text("Ambient Config");
  UI::DrawAmbientConfig(point_light.GetAmbientConfigRef());
  ImGui::Text("Point Config");
  UI::DrawPointConfig(point_light.GetPointConfigRef());
  ImGui::Text("Diffuse Config");
  UI::DrawDiffuseConfig(point_light.GetDiffuseConfigRef());
}
void DrawEngineMonitorDataConst(const EngineMonitorData& engine_monitor_data) {
  ImGui::Text("Frame rate: %.1f", ImGui::GetIO().Framerate);
  ImGui::Text("Total frame time: %.1f",
              engine_monitor_data.frame_data.frame_time);
  if (ImGui::TreeNode("Layers")) {
    for (auto& ld : engine_monitor_data.frame_data.layer_data) {
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
  for (auto& sd : engine_monitor_data.frame_data.structure_data) {
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
}
}  // namespace UI