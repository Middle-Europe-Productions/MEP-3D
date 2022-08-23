#include <MEP-3D/template/util_common_draw.hpp>

namespace UI {
void DrawAmbientConfig(AmbientConfig& config) {}
void DrawDiffuseConfig(DiffuseConfig& config) {}
void DrawDirectionalLight(DirectionalLight& directional_light) {}
void DrawSpotConfig(SpotConfig& point_config) {}
void DrawSpotLight(SpotLight& point_light) {}
void DrawPointConfig(PointConfig& point_config) {}
void DrawPointLight(PointLight& point_light) {}
void DrawModel(Model& model) {}
int DrawShaderComboMenu(std::vector<std::unique_ptr<Shader>>& array,
    int selected) {
    return -1;
}
void DrawEngineMonitorDataConst(const EngineMonitorData& engine_monitor_data) {}
void DrawAssetControllerConst(const AssetController& layer_controller) {}
}  // namespace UI
