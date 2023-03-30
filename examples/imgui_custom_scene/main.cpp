#include <MEP-3D/mep-3d.hpp>

#include <imgui.h>

#include <MEP-3D/template/ui_element.hpp>
#include <MEP-3D/template/ui_handlers_impl.hpp>

namespace {
constexpr char kSceneName[] = "main-scene";
}

using namespace mep;
class DefaultScene final : public Scene {
 public:
  DefaultScene() : Scene(kSceneName) {}
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void OnDraw(RenderTarget& render_target) override {}
  virtual void OnReceive(const IdentityView& id,
                         const nlohmann::json& message) {
    LOG(INFO) << "Message received: " << id.ToString() << ", " << message;
  }
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
   ],
   "popup":[
    {
      "return": "my_popup"
    } 
   ]
})";

enum MyOwnEnum { Test = UI_ELEMENT_COUNT + 1, Popup };

struct Data : public UI_HANDLER_BASE(SceneUILayer) {
  int data = 0;
  void Reset() { data = 0; }
};

UI_HANDLER_D(MyOwnEnum, Test, SceneUILayer, Data) {
  ImGui::Text("Hello");
  if (ImGui::Button("Button")) {
    GetContext()->Send(
        mep::IdentityView(std::nullopt, kSceneName, std::nullopt),
        std::to_string(data++));
  }
  if (ImGui::Button("Open popup")) {
    UI::Drawer::OpenPopup(MyOwnEnum::Popup);
  }
  if (ImGui::Button("Reset")) {
    Reset();
  }
};

UI_HANDLER(MyOwnEnum, Popup, SceneUILayer) {
  ImGui::Text("Hello");
};

int main(int argc, char* argv[]) {
  ADD_UI_ELEMENT(MyOwnEnum::Test, "test");
  ADD_UI_ELEMENT(MyOwnEnum::Popup, "my_popup");
  utils::Init(argc, argv);
  auto window = Window::GetInstance({{1280, 720}, "Custom Scene"});
  window->Init();
  auto engine = utils::CreateEngineWithSceneUI(
      std::move(window), std::make_unique<DefaultScene>(),
      SceneUILayer::Create(kMyRuntimeConfig));
  engine->Run();
}