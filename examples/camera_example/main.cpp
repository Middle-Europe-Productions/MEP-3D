#include <MEP-3D/mep-3d.hpp>
#include <MEP-3D/template/shaders_preset.hpp>

#include <imgui.h>

class CameraScene final : public Scene {
 public:
  CameraScene() {}
  void OnAttach() override {
    auto& window = GetEngine()->GetWindow();
    if (!window) {
      LOG(ERROR) << "Window does not exist";
      return;
    }
    // Camera config
    view_ = std::make_unique<PerspectiveView>(PerspectiveView::Config(
        {glm::radians(45.0f), window->GetAspectRation(), 0.1f, 10000.0f}));
    auto camera =
        std::make_unique<PerspectiveCamera>(PerspectiveCameraConfig::Create());
    // Init window
    window->AddView(view_.get());
    window->AddCamera(camera.get());
    window->AddObserver(camera.get());
    // Load shader
    shader_ = std::make_unique<Shader>();
    shader_->CreateFromString(shaders::kBasicVertexShader,
                              shaders::kBasicFragmentShader);
    shaders::CacheDefaultUnifroms(*shader_);

    triangle_ = std::make_unique<Pyramid>();
    triangle_->Bind(shader_.get());
    Attach(std::move(camera));
  }
  void OnDetach() override {}
  void OnUpdate(float time_delta) override { UpdateAll(time_delta); }
  void OnDraw(RenderTarget& render_target) override {
    shader_->Use();
    triangle_->Draw(render_target);
    shader_->Stop();
  }

 public:
  std::unique_ptr<PerspectiveView> view_;
  std::unique_ptr<Shader> shader_;
  std::unique_ptr<Pyramid> triangle_;
};

int main(int argc, char* argv[]) {
  utils::InitLogging(argc, argv);
  auto window = Window::GetInstance({{1280, 720}, "Camera example"});
  window->Init();
  auto engine = utils::CreateEngineWithSceneUI(std::move(window),
                                               std::make_unique<CameraScene>(),
                                               SceneUILayer::Create());
  engine->Run();
}