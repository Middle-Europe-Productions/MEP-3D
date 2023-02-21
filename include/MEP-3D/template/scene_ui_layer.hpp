#ifndef SCENE_UI_LAYER_HPP
#define SCENE_UI_LAYER_HPP

#include <MEP-3D/layer.hpp>
#include <MEP-3D/scene.hpp>
#include <MEP-3D/scene_ui_parser.hpp>
#include <MEP-3D/template/ui_element.hpp>

namespace mep {
class SceneUILayer : public Layer, public SceneObserver {
 public:
  void OnSceneAttach(Scene* scene) override;
  void OnSceneDetach() override;
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
}  // namespace mep

#endif