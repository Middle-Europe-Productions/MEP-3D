#include <MEP-3D/template/util_common_draw.hpp>

namespace UI {
void Drawer::DrawAmbientConfig(AmbientConfig& config) {}
void Drawer::DrawDiffuseConfig(DiffuseConfig& config) {}
void Drawer::DrawSpotConfig(SpotConfig& point_config) {}
void Drawer::DrawPointConfig(PointConfig& point_config) {}
void Drawer::DrawModelController(ModelController& model_controller) {}
void Drawer::DrawPerspectiveCamera(PerspectiveCamera& perspective_camera) {}

bool Drawer::DrawDirectionalLight(DirectionalLight& directional_light) {
  return true;
}

bool Drawer::DrawSpotLight(SpotLight& point_light) {
  return true;
}

bool Drawer::DrawPointLight(PointLight& point_light) {
  return true;
}

bool Drawer::DrawModel(Model& model) {
  return true;
}

bool Drawer::DrawShader(Shader& shader) {
  return true;
}

int Drawer::DrawShaderComboMenu(std::vector<std::unique_ptr<Shader>>& array,
                                int selected) {
  return -1;
}
int Drawer::DrawMaterialComboMenu(std::vector<std::unique_ptr<Material>>& array,
                                  int selected) {
  return -1;
}
int Drawer::DrawTextureComboMenu(std::vector<std::unique_ptr<Texture>>& array,
                                 int selected) {
  return -1;
}

void Drawer::DrawEngineMonitorDataConst(
    const EngineMonitorData& engine_monitor_data) {}
void Drawer::DrawAssetControllerConst(const AssetController& scene) {}
}  // namespace UI
