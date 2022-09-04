#ifndef UTIL_COMMON_DRAW_HPP
#define UTIL_COMMON_DRAW_HPP
#include <MEP-3D/asset_controller.hpp>
#include <MEP-3D/directional_light.hpp>
#include <MEP-3D/engine.hpp>
#include <MEP-3D/light_controller.hpp>
#include <MEP-3D/model.hpp>
#include <MEP-3D/shader.hpp>

namespace UI {
void DrawAmbientConfig(AmbientConfig& config);
void DrawDiffuseConfig(DiffuseConfig& config);
void DrawDirectionalLight(DirectionalLight& directional_light);
void DrawSpotConfig(SpotConfig& point_config);
void DrawSpotLight(SpotLight& point_light);
void DrawPointConfig(PointConfig& point_config);
void DrawPointLight(PointLight& point_light);
void DrawModelController(ModelController& model_controller);
void DrawModel(Model& model);
int DrawShaderComboMenu(std::vector<std::unique_ptr<Shader>>& array,
                        int selected);
int DrawMaterialComboMenu(std::vector<std::unique_ptr<Material>>& array,
                          int selected);
int DrawTextureComboMenu(std::vector<std::unique_ptr<Texture>>& array,
                         int selected);

void DrawEngineMonitorDataConst(const EngineMonitorData& engine_monitor_data);
void DrawAssetControllerConst(const AssetController& scene);

}  // namespace UI

#endif