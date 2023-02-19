#ifndef UTIL_COMMON_DRAW_HPP
#define UTIL_COMMON_DRAW_HPP
#include <MEP-3D/asset_controller.hpp>
#include <MEP-3D/directional_light.hpp>
#include <MEP-3D/engine.hpp>
#include <MEP-3D/light_controller.hpp>
#include <MEP-3D/model.hpp>
#include <MEP-3D/scene.hpp>
#include <MEP-3D/shader.hpp>
#include <MEP-3D/template/ui_element.hpp>
#include <MEP-3D/volume.hpp>

namespace UI {
class Drawer {
 public:
  // Interface elements
  static void DrawWindowInterface(Window& window, Scene& scene);
  // Editable elements
  static void DrawAmbientConfig(AmbientConfig& config);
  static void DrawDiffuseConfig(DiffuseConfig& config);
  static void DrawSpotConfig(SpotConfig& point_config);
  static void DrawPointConfig(PointConfig& point_config);
  static void DrawModelController(ModelController& model_controller);
  static void DrawCameraBase(CameraBase& camera_base);
  static void DrawPerspectiveCamera(PerspectiveCamera& perspective_camera);
  // Castable elements
  static void DrawCamera(CameraBase* camera_base);
  // Deletable elements (return false if object should be deleted)
  static bool DrawDirectionalLight(DirectionalLight& directional_light);
  static bool DrawSpotLight(SpotLight& point_light);
  static bool DrawPointLight(PointLight& point_light);
  static bool DrawModel(Model& model);
  static bool DrawShader(Shader& shader);
  static bool DrawVolume(Volume& volume);
  // Combo menus
  static int DrawShaderComboMenu(std::vector<std::unique_ptr<Shader>>& array,
                                 int selected);
  static int DrawMaterialComboMenu(
      std::vector<std::unique_ptr<Material>>& array,
      int selected);
  static int DrawTextureComboMenu(
      std::vector<std::unique_ptr<TextureBase>>& array,
      int selected);
  // Constant elements
  static void DrawEngineMonitorDataConst(
      const EngineMonitorData& engine_monitor_data);
  static void DrawAssetControllerConst(const AssetController& scene);

  // Utility
  static void OpenPopup(const std::string& name);
  static void OpenPopup(UI::Element name);
  static void OpenPopup(int id);

 private:
  Drawer() = delete;
  Drawer(const Drawer&) = delete;
  Drawer(Drawer&&) = delete;
};

}  // namespace UI

#endif