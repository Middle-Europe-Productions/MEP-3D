#ifndef UTIL_COMMON_DRAW_HPP
#define UTIL_COMMON_DRAW_HPP
#include <MEP-3D/asset_controller.hpp>
#include <MEP-3D/directional_light.hpp>
#include <MEP-3D/engine.hpp>
#include <MEP-3D/light_controller.hpp>
#include <MEP-3D/model.hpp>
#include <MEP-3D/shader.hpp>

namespace UI {
class Drawer {
 public:
  // Editable elements
  static void DrawAmbientConfig(AmbientConfig& config);
  static void DrawDiffuseConfig(DiffuseConfig& config);
  static void DrawSpotConfig(SpotConfig& point_config);
  static void DrawPointConfig(PointConfig& point_config);
  static void DrawModelController(ModelController& model_controller);
  static void DrawCameraBase(CameraBase& camera_base);
  static void DrawPerspectiveCamera(PerspectiveCamera& perspective_camera);
  // Deletable elements (return false if object should be deleted)
  static bool DrawDirectionalLight(DirectionalLight& directional_light);
  static bool DrawSpotLight(SpotLight& point_light);
  static bool DrawPointLight(PointLight& point_light);
  static bool DrawModel(Model& model);
  static bool DrawShader(Shader& shader);
  // Combo menus
  static int DrawShaderComboMenu(std::vector<std::unique_ptr<Shader>>& array,
                                 int selected);
  static int DrawMaterialComboMenu(
      std::vector<std::unique_ptr<Material>>& array,
      int selected);
  static int DrawTextureComboMenu(std::vector<std::unique_ptr<Texture>>& array,
                                  int selected);
  // Constant elements
  static void DrawEngineMonitorDataConst(
      const EngineMonitorData& engine_monitor_data);
  static void DrawAssetControllerConst(const AssetController& scene);

 private:
  Drawer() = delete;
  Drawer(const Drawer&) = delete;
  Drawer(Drawer&&) = delete;
};

}  // namespace UI

#endif