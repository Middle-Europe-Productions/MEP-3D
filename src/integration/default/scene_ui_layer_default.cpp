#include <MEP-3D/template/scene_ui_layer.hpp>

namespace mep {
class SceneUILayerDefault : public SceneUILayer {
 public:
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void OnDraw(RenderTarget& render_target) override {}

 private:
  virtual bool ShouldIgnoreLayer() const override { return true; }
};

std::unique_ptr<SceneUILayer> SceneUILayer::Create(
    const std::string& runtime_configuration,
    std::unordered_map<int, Parser::Callback> handlers,
    Parser::Method handler_attach_method) {
  return std::make_unique<SceneUILayerDefault>();
}
}  // namespace mep
