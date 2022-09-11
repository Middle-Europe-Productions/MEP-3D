#include <imgui.h>
#include <functional>

#include <MEP-3D/scene_ui_parser.hpp>
#include <MEP-3D/template/scene_ui_layer.hpp>
#include <MEP-3D/template/util_common_draw.hpp>
#include <MEP-3D/utils.hpp>

#include "model_factory_imgui.hpp"
#include "point_light_factory_imgui.hpp"
#include "scene_ui_parser_imgui.hpp"
#include "spot_light_factory_imgui.hpp"

namespace {
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
  MenuActionCount
};

constexpr char kDefaultSceneRuntimeConfig[] = R"({
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
   ],
   "scene":[
    {
      "name":"Items",
      "return": [
        {
          "name":"Light",
          "return":[
            {
              "name":"Directional Light",
              "return":7
            },
            {
              "name":"Point Light",
              "return":9
            },
            {
              "name":"Spot Light",
              "return":8
            }
          ]
        },
        {
          "name":"Models",
          "return":10
        },
        {
          "name":"Voxels",
          "return":-1
        }
      ]
    } 
   ]
})";
};  // namespace

class SceneUILayerImGUI : public SceneUILayer {
 public:
  SceneUILayerImGUI(
      const std::string& runtime_configuration,
      std::unordered_map<int, SceneUIParser::Callback> handlers = {},
      SceneUIParser::Method handler_attach_method =
          SceneUIParser::Method::FillAndOverride)
      : menu_action_(MenuAction::None) {
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
    if (!utils::Contains(handler_attach_method,
                         SceneUIParser::Method::DoNotUseDefault)) {
      InitDefaultHandler();
      menu_.MergeHandler(handlers, handler_attach_method);
    } else {
      menu_.SetHandler(handlers);
    }
    menu_.Parse(runtime_configuration.c_str());
    LOG(INFO) << __FUNCTION__;
  }
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}

  void DrawModelMenu() {
    if (GetScenePtr()->GetModels().empty()) {
      ImGui::Text("You do not have any models yet!");
      return;
    }
    auto& array = GetScenePtr()->GetModels();
    std::queue<std::vector<std::unique_ptr<Model>>::iterator> to_remove;
    for (auto it = array.begin(); it != array.end();) {
      if (ImGui::TreeNode(it->get()->Identity::GetName().c_str())) {
        if (!UI::DrawModel(*it->get())) {
          to_remove.push(it);
        }
        ImGui::Separator();
        ImGui::TreePop();
      }
      it++;
    }
    while (!to_remove.empty()) {
      array.erase(to_remove.front());
      to_remove.pop();
    }
  }

  void OnDraw(RenderTarget& render_target) override {
    if (!GetScenePtr())
      return;
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::Begin("Dockspace", NULL, window_flags);
    ImGui::PopStyleVar(3);
    ImGui::DockSpace(ImGui::GetID("master_dockspace"), ImVec2(0.0f, 0.0f),
                     ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();

    ImGui::Begin("Scene", NULL, ImGuiWindowFlags_MenuBar);
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
      default:
        menu_action_ = MenuAction::None;
    };
    DrawFactory("New Point Light", factory[0].get());
    DrawFactory("New Spot Light", factory[1].get());
    DrawFactory("New Model", factory[2].get());
    menu_.Draw();
    ImGui::End();
  }
  void InitDefaultHandler() {
    menu_.SetHandler(
        {{static_cast<int>(MenuAction::None),
          [this]() { ImGui::Text("Not implemented"); }},
         {static_cast<int>(MenuAction::AddPointLight),
          [this]() { this->menu_action_ = MenuAction::AddPointLight; }},
         {static_cast<int>(MenuAction::AddSpotLight),
          [this]() { this->menu_action_ = MenuAction::AddSpotLight; }},
         {static_cast<int>(MenuAction::AddModel),
          [this]() { this->menu_action_ = MenuAction::AddModel; }},
         {static_cast<int>(MenuAction::DrawDirectionalLight),
          [this]() {
            for (auto& dl_ptr : GetScenePtr()->GetDirectionaLights()) {
              if (ImGui::TreeNode(dl_ptr->Identity::GetName().c_str())) {
                UI::DrawDirectionalLight(*dl_ptr.get());
                ImGui::Separator();
                ImGui::TreePop();
              }
            }
            ImGui::Separator();
            UI::DrawAssetControllerConst(
                *GetScenePtr()->GetSpotLightController());
          }},
         {static_cast<int>(MenuAction::DrawPointLight),
          [this]() {
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
              UI::DrawAssetControllerConst(
                  *GetScenePtr()->GetPointLightController());
            }
          }},
         {static_cast<int>(MenuAction::DrawSpotLight),
          [this]() {
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
              UI::DrawAssetControllerConst(
                  *GetScenePtr()->GetSpotLightController());
            }
          }},
         {static_cast<int>(MenuAction::DrawModelMenu),
          std::bind(&SceneUILayerImGUI::DrawModelMenu, this)}});
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

std::unique_ptr<SceneUILayer> SceneUILayer::Create(
    const std::string& runtime_configuration,
    std::unordered_map<int, SceneUIParser::Callback> handlers,
    SceneUIParser::Method handler_attach_method) {
  if (runtime_configuration.empty()) {
    return std::make_unique<SceneUILayerImGUI>(kDefaultSceneRuntimeConfig,
                                               handlers, handler_attach_method);
  }
  return std::make_unique<SceneUILayerImGUI>(runtime_configuration.c_str(),
                                             handlers, handler_attach_method);
}
