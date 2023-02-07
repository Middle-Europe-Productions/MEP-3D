#include <MEP-3D/mep-3d.hpp>

#include <imgui.h>

#include <MEP-3D/template/ui_element.hpp>
#include <MEP-3D/template/ui_handlers_impl.hpp>

namespace {
constexpr char kSceneName[] = "main-scene";
}

class DefaultScene final : public Scene {
 public:
  DefaultScene() : Scene(kSceneName) {}
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void OnDraw(RenderTarget& render_target) override {}
  virtual void OnReceive(const Identity& id, const std::string& message) {
    LOG(INFO) << "Message received: " << id.ToString() << ", " << message;
  }
};

constexpr char kMyRuntimeConfig[] = R"({
  "window": [
    {
      "name": "Volume Rendering",
      "return": {
        "scene": [
          {
            "name": "Volumes",
            "return": [
              {
              "name": "Volumes",
              "return": "draw_volume"
              },
              {
              "name": "Volume Controller",
              "return": "test"
              }
            ]
          }
        ],
        "popup": [
          {
            "return": "my_popup"
          }
        ]
      }
    }
  ]
})";

enum MyOwnEnum { Test = UI_ELEMENT_COUNT + 1, Popup };

UI_HANDLER(MyOwnEnum, Test, SceneUILayer) {
  if (ImGui::Button("Add Volume")) {
    LOG(INFO) << "Start";
    std::unique_ptr<Volume> volume = std::make_unique<Volume>();
    volume->LoadFromFile("beechnut_1024x1024x1546_uint16.raw",
                         {1024, 1024, 1546}, Texture3D::Type::BYTE_16);
    GetContext()->GetScenePtr()->Attach(std::move(volume));
    LOG(INFO) << "Finish";
  }
};

UI_HANDLER(MyOwnEnum, Popup, SceneUILayer) {
  ImGui::Text("Hello");
};

int main(int argc, char* argv[]) {
  ADD_UI_ELEMENT(MyOwnEnum::Test, "test");
  ADD_UI_ELEMENT(MyOwnEnum::Popup, "my_popup");
  utils::Init(argc, argv);
  auto window = Window::GetInstance({{1280, 720}, "Volume Rendering"});
  window->Init();
  auto engine = utils::CreateEngineWithSceneUI(
      std::move(window), std::make_unique<DefaultScene>(),
      SceneUILayer::Create(kMyRuntimeConfig));
  engine->Run();
}
