#include <MEP-3D/template/util_common_draw.hpp>
#include <MEP-3D/utils.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <numeric>

#include "imgui_addons.hpp"
namespace {
Color ToMepColor(ImVec4& im_color) {
  return Color(255 * im_color.x, 255 * im_color.y, 255 * im_color.z,
               255 * im_color.w);
}
ImVec4 FromMepColor(Color& color) {
  return ImVec4(color.Rf(), color.Gf(), color.Bf(), color.Af());
}

template <typename Element>
int DrawComboMenuFromMEP(std::vector<std::unique_ptr<Element>>& array,
                         int selected,
                         const char* type) {
  std::vector<const char*> ele;
  ele.push_back("");
  for (auto& element : array) {
    ele.push_back(element->GetName().c_str());
  }
  int index = selected + 1;
  if (ele.size() == 0) {
    return -1;
  }
  ImGui::Combo(type, &index, ele.data(), ele.size());
  return index - 1;
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
// Jump value
constexpr float kGlobalSlide = 0.05;
}  // namespace

namespace UI {
void Drawer::DrawAmbientConfig(AmbientConfig& config) {
  ImVec4 im_color = FromMepColor(config.color);
  ImGui::ColorEdit4("Light Color##2", (float*)&im_color,
                    ImGuiColorEditFlags_DisplayRGB);
  config.color = ToMepColor(im_color);
  ImGui::SliderFloat("Ambient Intensity", &config.intensity, 0.0f, 1.0f, "%.3f",
                     ImGuiSliderFlags_None);
}
void Drawer::DrawDiffuseConfig(DiffuseConfig& config) {
  if (config.direction.has_value()) {
    float v[3] = {config.direction.value().x_, config.direction.value().y_,
                  config.direction.value().z_};
    ImGui::DragFloat3("Position", v, kGlobalSlide, -FLT_MAX, FLT_MAX);
    config.direction.value().x_ = v[0];
    config.direction.value().y_ = v[1];
    config.direction.value().z_ = v[2];
  }
  ImGui::SliderFloat("Diffuse Intensity", &config.intensity, 0.0f, 1.0f, "%.3f",
                     ImGuiSliderFlags_None);
}
void Drawer::DrawSpotConfig(SpotConfig& point_config) {
  int deg = static_cast<int>(point_config.edge_deg);
  ImGui::DragScalar("Degree", ImGuiDataType_S32, &deg, 1.0, &kDegMin, &kDegMax,
                    "%i°");
  point_config.edge_deg = static_cast<float>(deg);
  float v[3] = {point_config.direction.x_, point_config.direction.y_,
                point_config.direction.z_};
  ImGui::SliderFloat3("Direction", v, -1.0, 1.0);
  point_config.direction.x_ = v[0];
  point_config.direction.y_ = v[1];
  point_config.direction.z_ = v[2];
}
void Drawer::DrawPointConfig(PointConfig& point_config) {
  float vl = 10.0;
  ImGui::DragFloat("Quadratic", &point_config.quadratic, 0.01f, 0.0f, FLT_MAX,
                   "%.3f", ImGuiSliderFlags_None);
  ImGui::DragFloat("Linear", &point_config.linear, 0.01f, 0.0f, FLT_MAX, "%.3f",
                   ImGuiSliderFlags_None);
  ImGui::DragFloat("Constant", &point_config.constant, 0.01f, 0.0f, FLT_MAX,
                   "%.3f", ImGuiSliderFlags_None);
  float v[3] = {point_config.position.x_, point_config.position.y_,
                point_config.position.z_};
  ImGui::DragFloat3("Position", v, kGlobalSlide, -FLT_MAX, FLT_MAX);
  point_config.position.x_ = v[0];
  point_config.position.y_ = v[1];
  point_config.position.z_ = v[2];
}

void Drawer::DrawCameraBase(CameraBase& camera_base) {
  ImGui::Text("Camera Base");
  ImGui::Separator();
  auto& position = camera_base.Get(CameraVariables::Position);
  ImGui::DragFloat3("Position", static_cast<float*>(&position.x), kGlobalSlide,
                    -FLT_MAX, FLT_MAX);
  auto norm_direction = camera_base.GetNormalizedDirection();
  ImGui::Text("Direciton: [%f, %f, %f]", norm_direction.x, norm_direction.y,
              norm_direction.z);
}

void Drawer::DrawPerspectiveCamera(PerspectiveCamera& perspective_camera) {
  DrawCameraBase(perspective_camera);
  ImGui::Text("Perspective Camera");
  ImGui::Separator();
  if (ImGui::DragFloat("Yaw: ", &perspective_camera.yaw_, 0.01f, 0.0f, FLT_MAX,
                       "%.3f", ImGuiSliderFlags_None)) {
    perspective_camera.Changed();
  }
  if (ImGui::DragFloat("Pitch: ", &perspective_camera.pitch_, 0.01f, -90.0f,
                       90.0, "%.3f", ImGuiSliderFlags_None)) {
    perspective_camera.Changed();
  }
  perspective_camera.Update();
  ImGui::DragFloat("Move speed: ", &perspective_camera.move_speed_, 0.01f, 0.0f,
                   FLT_MAX, "%.3f", ImGuiSliderFlags_None);
  ImGui::DragFloat("Turn speed: ", &perspective_camera.turn_speed_, 0.01f, 0.0f,
                   FLT_MAX, "%.3f", ImGuiSliderFlags_None);
  ImGui::Checkbox("Reverse X: ", &perspective_camera.reversed_x_axis_);
  ImGui::Checkbox("Reverse Y: ", &perspective_camera.reversed_y_axis_);
  ImGui::Text("Keymap: ");
  for (PerspectiveCameraActions it = PerspectiveCameraActions::Front;
       it != PerspectiveCameraActions::Count; it = utils::IncEnum(it)) {
    ImGui::Text("%s:", ToString(it).c_str());
    ImGui::SameLine();
    ImGui::Text("%s",
                KeyboardToString(
                    perspective_camera.controls_.keys[static_cast<int>(it)])
                    .c_str());
  }
}

bool Drawer::DrawDirectionalLight(DirectionalLight& directional_light) {
  ImGui::Text("Ambient Config");
  UI::Drawer::DrawAmbientConfig(directional_light.GetAmbientConfigRef());
  ImGui::Text("Diffuse Config");
  UI::Drawer::DrawDiffuseConfig(directional_light.GetDiffuseConfigRef());
  return true;
}
bool Drawer::DrawSpotLight(SpotLight& point_light) {
  ImGui::Text("Ambient Config");
  UI::Drawer::DrawAmbientConfig(point_light.GetAmbientConfigRef());
  ImGui::Text("Point Config");
  UI::Drawer::DrawPointConfig(point_light.GetPointConfigRef());
  ImGui::Text("Diffuse Config");
  UI::Drawer::DrawDiffuseConfig(point_light.GetDiffuseConfigRef());
  ImGui::Text("Spot Config");
  UI::Drawer::DrawSpotConfig(point_light.GetSpotConfigRef());
  return true;
}
bool Drawer::DrawPointLight(PointLight& point_light) {
  ImGui::Text("Ambient Config");
  UI::Drawer::DrawAmbientConfig(point_light.GetAmbientConfigRef());
  ImGui::Text("Point Config");
  UI::Drawer::DrawPointConfig(point_light.GetPointConfigRef());
  ImGui::Text("Diffuse Config");
  UI::Drawer::DrawDiffuseConfig(point_light.GetDiffuseConfigRef());
  return true;
}
bool Drawer::DrawModel(Model& model) {
  auto status = model.GetStatus();
  if (status == Status::NotImplemented) {
    ImGui::Text("Model module is not implemented!");
  } else {
    ImGui::Text("Draw ");
    ImGui::SameLine();
    std::string id =
        "##should_render_model" + std::to_string(model.GetGlobalId());
    ImGui::Checkbox(id.c_str(), &model.GetShouldDraw());
    ImGui::Text("Model Status ");
    ImGui::SameLine();
    ImGui::Text("%s", ToString(status).c_str());
    if (status == Status::Loading) {
      ImGui::Spinner("spinner", 10, 4, ImGui::GetColorU32(ImGuiCol_TabActive));
    } else if (status == Status::Avalible) {
      ImGui::Separator();
      Drawer::DrawModelController(model);
    }
    UI::Drawer::DrawAssetControllerConst(model);
  }
  if (ImGui::Button("Delete")) {
    return false;
  }
  return true;
}

bool Drawer::DrawShader(Shader& shader) {
  auto& shader_config = shader.GetShaderStatus();
  if (shader_config.created_from_file) {
    ImGui::Text("Shader created from file");
    ImGui::Separator();
    ImGui::Text("Vertex path: %s",
                shader_config.vertex_path.value_or("none").c_str());
    if (shader_config.fragment_path.has_value())
      ImGui::Text("Fragment path: %s",
                  shader_config.fragment_path.value_or("none").c_str());
    ImGui::Separator();
  } else {
    ImGui::Text("Shader created from memory");
    ImGui::Separator();
  }
  if (shader_config.is_vertex_compiled)
    ImGui::Text("Vertex shader compiled!");
  if (ImGui::TreeNode("Vertex shader code")) {
    ImGui::BeginChild("vertex_code",
                      ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30),
                      true, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Text("%s", shader_config.vertex_code.value_or("Not Found").c_str());
    ImGui::EndChild();
    ImGui::TreePop();
  }
  ImGui::Separator();
  if (shader_config.is_fragment_compiled)
    ImGui::Text("Fragment shader compiled!");
  if (ImGui::TreeNode("Fragment shader code")) {
    ImGui::BeginChild("fragment_code",
                      ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30),
                      true, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Text("%s",
                shader_config.fragment_code.value_or("Not Found").c_str());
    ImGui::EndChild();
    ImGui::TreePop();
  }
  ImGui::Separator();
  ImGui::Text("Cache");
  if (ImGui::TreeNode("User Defined Uniforms")) {
    auto& uniforms = shader.GetUserCacheForDebug();
    for (auto& node : uniforms) {
      ImGui::Text("ID: %i. Address: %i", node.first, node.second);
    }
    ImGui::TreePop();
  }
  if (ImGui::TreeNode("Cached Uniforms")) {
    auto& uniforms = shader.GetAutoCacheForDebug();
    for (auto& node : uniforms) {
      ImGui::Text("Uniform: %s. Address: %i", node.first.c_str(), node.second);
    }
    ImGui::TreePop();
  }
  return true;
}

void Drawer::DrawModelController(ModelController& model_controller) {
  float* model_ptr = (float*)glm::value_ptr(model_controller.GetModel());
  ImGui::Text("Position");
  ImGui::DragFloat("x", model_ptr + 12, kGlobalSlide, -FLT_MAX, FLT_MAX);
  ImGui::DragFloat("y", model_ptr + 13, kGlobalSlide, -FLT_MAX, FLT_MAX);
  ImGui::DragFloat("z", model_ptr + 14, kGlobalSlide, -FLT_MAX, FLT_MAX);
  ImGui::Separator();
  ImGui::Text("Rotation [deg]");
  const auto& rotation = model_controller.GetRotationTracker();
  float x_rotation = rotation[0];
  ImGui::DragFloat("Along x", &x_rotation, kGlobalSlide, -FLT_MAX, FLT_MAX,
                   "%.2f°");
  if (x_rotation != rotation[0]) {
    model_controller.Rotate(x_rotation - rotation[0], Axis::X);
  }
  float y_rotation = rotation[1];
  ImGui::DragFloat("Along y", &y_rotation, kGlobalSlide, -FLT_MAX, FLT_MAX,
                   "%.2f°");
  if (y_rotation != rotation[1]) {
    model_controller.Rotate(y_rotation - rotation[1], Axis::Y);
  }
  float z_rotation = rotation[2];
  ImGui::DragFloat("Along z", &z_rotation, kGlobalSlide, -FLT_MAX, FLT_MAX,
                   "%.2f°");
  if (z_rotation != rotation[2]) {
    model_controller.Rotate(z_rotation - rotation[2], Axis::Z);
  }
  ImGui::Separator();
  ImGui::Text("Scale");
  ImGui::DragFloat("x scale", model_ptr + 0, kGlobalSlide, -FLT_MAX, FLT_MAX);
  ImGui::DragFloat("y scale", model_ptr + 5, kGlobalSlide, -FLT_MAX, FLT_MAX);
  ImGui::DragFloat("z scale", model_ptr + 10, kGlobalSlide, -FLT_MAX, FLT_MAX);

  ImGui::Separator();
  ImGui::Text("Transformation matrix");
  ImGui::DragFloat4("##row_1", model_ptr, kGlobalSlide, -FLT_MAX, FLT_MAX);
  ImGui::DragFloat4("##row_2", model_ptr + 4, kGlobalSlide, -FLT_MAX, FLT_MAX);
  ImGui::DragFloat4("##row_3", model_ptr + 8, kGlobalSlide, -FLT_MAX, FLT_MAX);
  ImGui::DragFloat4("##row_4", model_ptr + 12, kGlobalSlide, -FLT_MAX, FLT_MAX);
}

int Drawer::DrawShaderComboMenu(std::vector<std::unique_ptr<Shader>>& array,
                                int selected) {
  return DrawComboMenuFromMEP(array, selected, "##shader");
}

int Drawer::DrawMaterialComboMenu(std::vector<std::unique_ptr<Material>>& array,
                                  int selected) {
  return DrawComboMenuFromMEP(array, selected, "##material");
}

int Drawer::DrawTextureComboMenu(std::vector<std::unique_ptr<Texture>>& array,
                                 int selected) {
  return DrawComboMenuFromMEP(array, selected, "##texture");
}

void Drawer::DrawEngineMonitorDataConst(
    const EngineMonitorData& engine_monitor_data) {
  ImGui::Text("Frame rate: %.1f", ImGui::GetIO().Framerate);
  ImGui::Text("Total frame time: %.1f",
              engine_monitor_data.frame_data.frame_time);
  if (ImGui::TreeNode("Layers")) {
    for (auto& ld : engine_monitor_data.frame_data.layer_data) {
      if (ImGui::TreeNode(ld.layer_name.c_str())) {
        ImGui::Text("%s", ld.identity.ToString().c_str());
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
          ImGui::Text("%s", ld.identity.ToString().c_str());
          ImGui::Text("Layer draw time: %.3f", ld.layer_draw_time_ms);
          ImGui::Text("Layer update time: %.3f", ld.layer_update_time_ms);
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
  }
}
void Drawer::DrawAssetControllerConst(const AssetController& scene) {
  if (auto* texture = scene.Get<Texture>()) {
    ImGui::Text("Texture: ");
    ImGui::SameLine();
    ImGui::Text("%s", texture->GetName().c_str());
  }
  if (auto* shader = scene.Get<Shader>()) {
    ImGui::Text("Shader: ");
    ImGui::SameLine();
    ImGui::Text("%s", shader->GetName().c_str());
  }
  if (auto* model = scene.Get<Material>()) {
    ImGui::Text("Material: ");
    ImGui::SameLine();
    ImGui::Text("%s", model->GetName().c_str());
  }
}
}  // namespace UI