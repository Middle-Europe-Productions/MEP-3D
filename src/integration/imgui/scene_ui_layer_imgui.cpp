#include <imgui.h>
#include <functional>

#include <MEP-3D/scene_ui_parser.hpp>
#include <MEP-3D/template/scene_ui_layer.hpp>
#include <MEP-3D/template/util_common_draw.hpp>

#include "model_factory_imgui.hpp"
#include "point_light_factory_imgui.hpp"
#include "scene_ui_parser_imgui.hpp"
#include "spot_light_factory_imgui.hpp"

namespace {
enum MenuAction {
  None = -1,
  AddPointLight = 0,
  AddSpotLight = 1,
  AddModel,
  AddShader,
  AddImage,
  AddTexture,
  AddMaterial,
  MenuActionCount
};
constexpr char kDefaultSceneConfig[] = R"({
   "menu":[
      {
        "name":"New",
        "return":[
          {
             "name":"Light",
             "return":[
              {
                "name":"Point Light",
                "return":0
              },
              {
                "name":"Spot Light",
                "return":1
              }
            ]
          },
          {
            "name":"Model",
            "return":2
          }
        ]
      }
   ]
})";
};  // namespace

class SceneUILayerImGUI : public SceneUILayer {
 public:
  SceneUILayerImGUI(const std::string& config) {
    factory.emplace_back(std::make_unique<PointLightFactoryImGui>(
        std::bind(&SceneUILayerImGUI::AddPointLight, this,
                  std::placeholders::_1),
        std::bind(&SceneUILayerImGUI::RemovePointLight, this,
                  std::placeholders::_1)));
    factory.emplace_back(std::make_unique<SpotLightFactoryImGui>(
        std::bind(&SceneUILayerImGUI::AddSpotLight, this,
                  std::placeholders::_1),
        std::bind(&SceneUILayerImGUI::RemoveSpotLight, this,
                  std::placeholders::_1)));
    factory.emplace_back(std::make_unique<ModelFactoryImGui>(
        std::bind(&SceneUILayerImGUI::AddModel, this, std::placeholders::_1),
        std::bind(&SceneUILayerImGUI::RemoveModel, this,
                  std::placeholders::_1)));
    menu_.Parse(config.c_str());
    LOG(INFO) << __FUNCTION__;
  }
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void DrawMenuBar() {
    menu_action_ = static_cast<MenuAction>(menu_.DrawWithReplay());
  }
  void DrawLightMenu() {
    if (ImGui::CollapsingHeader("Directional Light")) {
      for (auto& dl_ptr : GetScenePtr()->GetDirectionaLights()) {
        if (ImGui::TreeNode(dl_ptr->Identity::GetName().c_str())) {
          UI::DrawDirectionalLight(*dl_ptr.get());
          ImGui::Separator();
          ImGui::TreePop();
        }
      }
      ImGui::Separator();
      UI::DrawAssetControllerConst(*GetScenePtr()->GetSpotLightController());
    }
    if (ImGui::CollapsingHeader("Spot Light")) {
      if (GetScenePtr()->GetSpotLightController()) {
        for (auto& sl_ptr :
             GetScenePtr()->GetSpotLightController()->GetContainer()) {
          if (ImGui::TreeNode(sl_ptr->Identity::GetName().c_str())) {
            UI::DrawSpotLight(*sl_ptr.get());
            ImGui::Separator();
            ImGui::TreePop();
          }
        }
        ImGui::Separator();
        UI::DrawAssetControllerConst(*GetScenePtr()->GetSpotLightController());
      }
    }
    if (ImGui::CollapsingHeader("Point Light")) {
      if (GetScenePtr()->GetPointLightController()) {
        for (auto& pl_ptr :
             GetScenePtr()->GetPointLightController()->GetContainer()) {
          if (ImGui::TreeNode(pl_ptr->Identity::GetName().c_str())) {
            UI::DrawPointLight(*pl_ptr.get());
            ImGui::Separator();
            ImGui::TreePop();
          }
        }
        ImGui::Separator();
        UI::DrawAssetControllerConst(*GetScenePtr()->GetSpotLightController());
      }
    }
  }

  void DrawMenu() {
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("Items", tab_bar_flags)) {
      if (ImGui::BeginTabItem("Light")) {
        DrawLightMenu();
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Models")) {
        DrawModelMenu();
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Voxels")) {
        ImGui::Text("Not implemented");
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
    ImGui::Separator();
  }

  void DrawModelMenu() {
    for (auto& model : GetScenePtr()->GetModels()) {
      if (ImGui::TreeNode(model->Identity::GetName().c_str())) {
        UI::DrawModel(*model.get());
        ImGui::Separator();
        ImGui::TreePop();
      }
    }
  }

  void OnDraw(RenderTarget& render_target) override {
    if (!GetScenePtr())
      return;
    ImGui::Begin("Scene", NULL, ImGuiWindowFlags_MenuBar);
    DrawMenuBar();
    switch (menu_action_) {
      case MenuAction::AddPointLight:
        ImGui::OpenPopup("New Point Light");
        menu_action_ = MenuAction::None;
        break;
      case MenuAction::AddSpotLight:
        ImGui::OpenPopup("New Spot Light");
        menu_action_ = MenuAction::None;
        break;
      case MenuAction::AddModel:
        ImGui::OpenPopup("New Model");
        menu_action_ = MenuAction::None;
        break;
    };
    DrawFactory("New Point Light", factory[0].get());
    DrawFactory("New Spot Light", factory[1].get());
    DrawFactory("New Model", factory[2].get());
    DrawMenu();
    ImGui::End();
  }
  ~SceneUILayerImGUI() { LOG(INFO) << __FUNCTION__; }

 private:
  template <typename What>
  void DrawFactory(const std::string& name, What* factory) {
    if (ImGui::BeginPopup(name.c_str(), ImGuiWindowFlags_MenuBar)) {
      factory->ImGUIDraw(*GetScenePtr());
      ImGui::EndPopup();
    } else {
      factory->Remove();
    }
  }
  PointLight* AddPointLight(PointLightPtr point_light) {
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    auto& cont = GetScenePtr()->GetPointLightController();
    if (!cont)
      return nullptr;
    auto it = cont->MakeAndBind(std::move(point_light));
    if (cont->IsValid(it)) {
      return it->get();
    }
    return nullptr;
  }
  SpotLight* AddSpotLight(SpotLightPtr spot_light) {
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    auto& cont = GetScenePtr()->GetSpotLightController();
    if (!cont) {
      LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
      return nullptr;
    }
    auto it = cont->MakeAndBind(std::move(spot_light));
    if (cont->IsValid(it)) {
      return it->get();
    }
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    return nullptr;
  }
  Model* AddModel(ModelPtr model) {
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    GetScenePtr()->GetModels().emplace_back(std::move(model));
    return GetScenePtr()->GetModels().back().get();
  }
  void RemovePointLight(const Identity& id) {
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    auto& cont = GetScenePtr()->GetPointLightController();
    if (!cont)
      return;
    cont->Remove(id);
  }
  void RemoveSpotLight(const Identity& id) {
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    auto& cont = GetScenePtr()->GetSpotLightController();
    if (!cont)
      return;
    cont->Remove(id);
  }
  void RemoveModel(const Identity& id) {
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    auto& cont = GetScenePtr()->GetModels();
    cont.erase(std::remove_if(cont.begin(), cont.end(),
                              [&id](const auto& in) {
                                return in->GetGlobalId() == id.GetGlobalId();
                              }),
               cont.end());
  }
  virtual bool ShouldIgnoreLayer() const override { return false; }
  std::vector<std::unique_ptr<ElementFactoryImGuiBase>> factory;
  MenuAction menu_action_;
  SceneUIParserImGui menu_;
};

std::unique_ptr<SceneUILayer> SceneUILayer::Create() {
  return std::make_unique<SceneUILayerImGUI>(kDefaultSceneConfig);
}

std::unique_ptr<SceneUILayer> SceneUILayer::Create(const std::string& config) {
  return std::make_unique<SceneUILayerImGUI>(config.c_str());
}
