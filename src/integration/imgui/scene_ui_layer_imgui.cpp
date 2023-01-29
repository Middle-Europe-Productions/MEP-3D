#include <imgui.h>
#include <functional>

#include <MEP-3D/scene_ui_parser.hpp>
#include <MEP-3D/template/scene_ui_layer.hpp>
#include <MEP-3D/template/util_common_draw.hpp>
#include <MEP-3D/utils.hpp>

#include "imgui_addons.hpp"
#include "model_factory_imgui.hpp"
#include "point_light_factory_imgui.hpp"
#include "scene_ui_layer_handler_context.hpp"
#include "scene_ui_parser_imgui.hpp"
#include "spot_light_factory_imgui.hpp"

namespace {

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
                "return":"add_point_light"
              },
              {
                "name":"Spot Light",
                "return":"add_spot_light"
              }
            ]
          },
          {
            "name":"Model",
            "return":"add_model"
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
              "return":"draw_directional_light"
            },
            {
              "name":"Point Light",
              "return":"draw_point_light"
            },
            {
              "name":"Spot Light",
              "return":"draw_spot_light"
            }
          ]
        },
        {
          "name":"Models",
          "return":"draw_model_menu"
        },
        {
          "name":"Voxels",
          "return":-1
        },
        {
          "name":"Shader",
          "return":"draw_shader"
        },
        {
          "name":"Cameras",
          "return":"draw_camera"
        },
        {
          "name":"Window",
          "return":"draw_window"
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
      : menu_action_(UI::Element::None) {
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
  void OnAttach() override { ImGui::ApplyMepStyle(); }
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}

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
      case UI::Element::AddPointLight:
        ImGui::OpenPopup("New Point Light");
        menu_action_ = UI::Element::None;
        break;
      case UI::Element::AddSpotLight:
        ImGui::OpenPopup("New Spot Light");
        menu_action_ = UI::Element::None;
        break;
      case UI::Element::AddModel:
        ImGui::OpenPopup("New Model");
        menu_action_ = UI::Element::None;
        break;
      default:
        menu_action_ = UI::Element::None;
    };
    DrawFactory("New Point Light", factory[0].get());
    DrawFactory("New Spot Light", factory[1].get());
    DrawFactory("New Model", factory[2].get());
    menu_.Draw();
    ImGui::End();
  }
  void InitDefaultHandler() {
    menu_.SetHandler(
        GET_UI_CONTEXT(SceneUILayer).GetHandlerMapWithContext(this));
    /*
menu_.SetHandler(
    {{static_cast<int>(UI::Element::None),
      [this]() { ImGui::Text("Not implemented"); }},
     {static_cast<int>(UI::Element::AddPointLight),
      [this]() { this->menu_action_ = UI::Element::AddPointLight; }},
     {static_cast<int>(UI::Element::AddSpotLight),
      [this]() { this->menu_action_ = UI::Element::AddSpotLight; }},
     {static_cast<int>(UI::Element::AddModel),
      [this]() { this->menu_action_ = UI::Element::AddModel; }},
     {static_cast<int>(UI::Element::DrawDirectionalLight),
      [this]() {
        for (auto& dl_ptr : GetScenePtr()->GetDirectionaLights()) {
          if (ImGui::TreeNode(dl_ptr->Identity::GetName().c_str())) {
            UI::Drawer::DrawDirectionalLight(*dl_ptr.get());
            ImGui::Separator();
            ImGui::TreePop();
          }
        }
      }},
     {static_cast<int>(UI::Element::DrawPointLight),
      [this]() {
        if (GetScenePtr()->GetPointLightController()) {
          for (auto& pl_ptr :
               GetScenePtr()->GetPointLightController()->GetContainer()) {
            if (ImGui::TreeNode(pl_ptr->Identity::GetName().c_str())) {
              UI::Drawer::DrawPointLight(*pl_ptr.get());
              ImGui::Separator();
              ImGui::TreePop();
            }
          }
          ImGui::Separator();
          UI::Drawer::DrawAssetControllerConst(
              *GetScenePtr()->GetPointLightController());
        }
      }},
     {static_cast<int>(UI::Element::DrawSpotLight),
      [this]() {
        if (GetScenePtr()->GetSpotLightController()) {
          for (auto& sl_ptr :
               GetScenePtr()->GetSpotLightController()->GetContainer()) {
            if (ImGui::TreeNode(sl_ptr->Identity::GetName().c_str())) {
              UI::Drawer::DrawSpotLight(*sl_ptr.get());
              ImGui::Separator();
              ImGui::TreePop();
            }
          }
          ImGui::Separator();
          UI::Drawer::DrawAssetControllerConst(
              *GetScenePtr()->GetSpotLightController());
        }
      }},
     {static_cast<int>(UI::Element::DrawShader),
      [this]() {
        for (auto& sh_ptr : GetScenePtr()->GetShaders()) {
          if (ImGui::TreeNode(sh_ptr->Identity::GetName().c_str())) {
            UI::Drawer::DrawShader(*sh_ptr.get());
            ImGui::Separator();
            ImGui::TreePop();
          }
        }
      }},
     {static_cast<int>(UI::Element::DrawModelMenu),
      std::bind(&SceneUILayerImGUI::DrawModelMenu, this)},
     {static_cast<int>(UI::Element::DrawCamera),
      std::bind(&SceneUILayerImGUI::DrawCamera, this)},
     {static_cast<int>(UI::Element::DrawWindow), [this]() {
        DCHECK(GetEngine());
        DCHECK(GetEngine()->GetWindow());
        DCHECK(GetScenePtr());
        UI::Drawer::DrawWindowInterface(*GetEngine()->GetWindow(),
                                        *GetScenePtr());
      }}});*/
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
  UI::Element menu_action_;
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
