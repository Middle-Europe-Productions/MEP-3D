#include <MEP-3D/mep-3d.hpp>

#include <imgui.h>

class DefaultScene final : public Scene {
 public:
  DefaultScene() {}
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void OnDraw(RenderTarget& render_target) override {}
};

constexpr char kMyRuntimeConfig[] = R"({
   "scene":[
    {
      "name":"Items",
      "return": [
        {
          "name":"Example",
          "return":[
            {
              "name":"First menu",
              "return":1
            }
          ]
        }
      ]
    } 
   ]
})";

const auto kMyHandler = []() {
  ImGui::Text("Hello");
  static bool pressed = false;
  if (ImGui::Button("Button")) {
    pressed = !pressed;
  }
  if (pressed) {
    ImGui::Text("World!");
  }
};

int main(int argc, char* argv[]) {
  utils::InitMepLogging(argv[0]);
  auto window = Window::GetInstance({{1280, 720}, "Custom Scene"});
  window->Init();
  auto engine = utils::CreateEngineWithSceneUI(
      std::make_unique<DefaultScene>(),
      SceneUILayer::Create(kMyRuntimeConfig, {{1, kMyHandler}},
                           SceneUIParser::Method::DoNotUseDefault));
  engine->AttachWindow(std::move(window));
  engine->Run();
}