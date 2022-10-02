#ifndef SCENE_UI_LAYER_HPP
#define SCENE_UI_LAYER_HPP

#include <MEP-3D/layer.hpp>
#include <MEP-3D/scene.hpp>
#include <MEP-3D/scene_ui_parser.hpp>

class SceneUILayer : public Layer, public SceneObserver {
 public:
  enum MenuAction {
    None = -1,
    AddPointLight = 0,
    AddSpotLight = 1,
    AddModel = 2,
    AddShader = 3,
    AddImage = 4,
    AddTexture = 5,
    AddMaterial = 6,
    DrawDirectionalLight = 7,
    DrawSpotLight = 8,
    DrawPointLight = 9,
    DrawModelMenu = 10,
    DrawShader = 11,
    MenuActionCount
  };
  void OnAttach(Scene* scene);
  void OnDetach();
  Scene* GetScenePtr();
  static std::unique_ptr<SceneUILayer> Create(
      const std::string& runtime_configuration = "",
      std::unordered_map<int, SceneUIParser::Callback> handlers = {},
      SceneUIParser::Method handler_attach_method =
          SceneUIParser::Method::FillAndOverride);
  virtual ~SceneUILayer();

 protected:
  Scene* scene_ptr_;
  SceneUILayer();
};

#endif