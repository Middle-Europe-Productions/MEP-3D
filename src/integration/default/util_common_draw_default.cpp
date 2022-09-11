#include <MEP-3D/template/util_common_draw.hpp>

namespace UI {
void DrawAmbientConfig(AmbientConfig& config) {}
void DrawDiffuseConfig(DiffuseConfig& config) {}
void DrawSpotConfig(SpotConfig& point_config) {}
void DrawPointConfig(PointConfig& point_config) {}
void DrawModelController(ModelController& model_controller) {}

bool DrawDirectionalLight(DirectionalLight& directional_light) {
  return true;
}
bool DrawSpotLight(SpotLight& point_light) {
  return true;
}
bool DrawPointLight(PointLight& point_light) {
  return true;
}
bool DrawModel(Model& model) {
  return true;
}
int DrawShaderComboMenu(std::vector<std::unique_ptr<Shader>>& array,
                        int selected) {
  return -1;
}
int DrawMaterialComboMenu(std::vector<std::unique_ptr<Material>>& array,
                          int selected) {
  return -1;
}
int DrawTextureComboMenu(std::vector<std::unique_ptr<Texture>>& array,
                         int selected) {
  return -1;
}

void DrawEngineMonitorDataConst(const EngineMonitorData& engine_monitor_data) {}
void DrawAssetControllerConst(const AssetController& scene) {}
}  // namespace UI
