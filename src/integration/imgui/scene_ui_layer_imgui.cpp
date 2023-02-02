#include <imgui.h>
#include <functional>

#include <MEP-3D/scene_ui_parser.hpp>
#include <MEP-3D/template/scene_ui_layer.hpp>
#include <MEP-3D/template/scene_ui_layer_handler_context.hpp>
#include <MEP-3D/template/util_common_draw.hpp>
#include <MEP-3D/utils.hpp>

#include "imgui_addons.hpp"
#include "scene_ui_parser_imgui.hpp"

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
                "return":"open_point_light_popup"
              },
              {
                "name":"Spot Light",
                "return":"open_spot_light_popup"
              }
            ]
          },
          {
            "name":"Model",
            "return":"open_model_popup"
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
   ],
   "popup":[
    {
      "return":"point_light_popup"
    },
    {
      "return":"spot_light_popup"
    },
    {
      "return":"model_popup"
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
    menu_.Draw();
    ImGui::End();
  }
  void InitDefaultHandler() {
    menu_.SetHandler(
        GET_UI_CONTEXT(SceneUILayer).GetHandlerMapWithContext(this));
  }

  ~SceneUILayerImGUI() { LOG(INFO) << __FUNCTION__; }

 private:
  virtual bool ShouldIgnoreLayer() const override { return false; }
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
