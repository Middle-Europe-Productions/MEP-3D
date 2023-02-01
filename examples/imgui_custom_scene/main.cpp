#include <MEP-3D/mep-3d.hpp>

#include <imgui.h>

#include <MEP-3D/template/ui_element.hpp>
#include <MEP-3D/template/ui_handlers_impl.hpp>

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
              "return":"test"
            }
          ]
        }
      ]
    } 
   ]
})";

enum MyOwnEnum { Test = UI_ELEMENT_COUNT + 1 };

UI_HANDLER(MyOwnEnum, Test, SceneUILayer) {
  ImGui::Text("Hello");
  static bool pressed = false;
  if (ImGui::Button("Button")) {
    pressed = !pressed;
    GetContext()->Send("target", "hello");
  }
  if (pressed) {
    ImGui::Text("World!");
  }
};

int main(int argc, char* argv[]) {
  ADD_UI_ELEMENT(MyOwnEnum::Test, "test");
  utils::Init(argc, argv);
  auto window = Window::GetInstance({{1280, 720}, "Custom Scene"});
  window->Init();
  auto engine = utils::CreateEngineWithSceneUI(
      std::move(window), std::make_unique<DefaultScene>(),
      SceneUILayer::Create(kMyRuntimeConfig));
  engine->Run();
}