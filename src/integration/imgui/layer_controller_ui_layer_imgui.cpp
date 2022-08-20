#include <imgui.h>
#include <functional>

#include <MEP-3D/template/layer_controller_ui_layer.hpp>
#include <MEP-3D/template/util_common_draw.hpp>

#include "point_light_factory_imgui.hpp"
#include "spot_light_factory_imgui.hpp"

namespace {
enum MenuAction { None, AddPointLight, AddSpotLight };
};  // namespace

class LayerControllerUILayerImGUI : public LayerControllerUILayer {
 public:
  LayerControllerUILayerImGUI()
      : point_light_fact_(
            std::bind(&LayerControllerUILayerImGUI::AddPointLight,
                      this,
                      std::placeholders::_1),
            std::bind(&LayerControllerUILayerImGUI::RemovePointLight,
                      this,
                      std::placeholders::_1)),
        spot_light_fact_(
            std::bind(&LayerControllerUILayerImGUI::AddSpotLight,
                      this,
                      std::placeholders::_1),
            std::bind(&LayerControllerUILayerImGUI::RemoveSpotLight,
                      this,
                      std::placeholders::_1)) {}
  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void DrawMenuBar() {
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("New")) {
        if (ImGui::BeginMenu("Light")) {
          if (ImGui::MenuItem("Point Light")) {
            menu_action_ = MenuAction::AddPointLight;
          }
          if (ImGui::MenuItem("Spot Light")) {
            menu_action_ = MenuAction::AddSpotLight;
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
  }
  void DrawLightMenu() {
    if (ImGui::CollapsingHeader("Directional Light")) {
      for (auto& dl_ptr : GetLayerControllerPtr()->GetDirectionaLights()) {
        if (ImGui::TreeNode(dl_ptr->Identity::GetName().c_str())) {
          UI::DrawDirectionalLight(*dl_ptr.get());
          ImGui::Separator();
          ImGui::TreePop();
        }
      }
    }
    if (ImGui::CollapsingHeader("Spot Light")) {
      if (GetLayerControllerPtr()->GetSpotLightController()) {
        for (auto& sl_ptr : GetLayerControllerPtr()
                                ->GetSpotLightController()
                                ->GetContainer()) {
          if (ImGui::TreeNode(sl_ptr->Identity::GetName().c_str())) {
            UI::DrawSpotLight(*sl_ptr.get());
            ImGui::Separator();
            ImGui::TreePop();
          }
        }
      }
    }
    if (ImGui::CollapsingHeader("Point Light")) {
      if (GetLayerControllerPtr()->GetPointLightController()) {
        for (auto& pl_ptr : GetLayerControllerPtr()
                                ->GetPointLightController()
                                ->GetContainer()) {
          if (ImGui::TreeNode(pl_ptr->Identity::GetName().c_str())) {
            UI::DrawPointLight(*pl_ptr.get());
            ImGui::Separator();
            ImGui::TreePop();
          }
        }
      }
    }
  }

  void DrawMenu() {
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
      if (ImGui::BeginTabItem("Light")) {
        DrawLightMenu();
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Models")) {
        ImGui::Text("Not implemented");
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
  void OnDraw(RenderTarget& render_target) override {
    if (!GetLayerControllerPtr())
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
    };
    DrawPointLightFactory();
    DrawSpotLightFactory();
    DrawMenu();
    ImGui::End();
  }

 private:
  void DrawPointLightFactory() {
    if (ImGui::BeginPopup("New Point Light", ImGuiWindowFlags_MenuBar)) {
      point_light_fact_.ImGUIDraw();
      ImGui::EndPopup();
    } else {
      point_light_fact_.Remove();
    }
  }
  void DrawSpotLightFactory() {
    if (ImGui::BeginPopup("New Spot Light", ImGuiWindowFlags_MenuBar)) {
      spot_light_fact_.ImGUIDraw();
      ImGui::EndPopup();
    } else {
      spot_light_fact_.Remove();
    }
  }
  PointLight* AddPointLight(PointLightPtr point_light) {
    auto& cont = GetLayerControllerPtr()->GetPointLightController();
    if (!cont)
      return nullptr;
    auto it = cont->MakeAndBind(std::move(point_light));
    if (cont->IsValid(it)) {
      return it->get();
    }
    return nullptr;
  }
  void RemovePointLight(const Identity& id) {
    auto& cont = GetLayerControllerPtr()->GetPointLightController();
    if (!cont)
      return;
    cont->Remove(id);
  }
  SpotLight* AddSpotLight(SpotLightPtr spot_light) {
    auto& cont = GetLayerControllerPtr()->GetSpotLightController();
    if (!cont) {
      LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
      return nullptr;
    }
    auto it = cont->MakeAndBind(std::move(spot_light));
    if (cont->IsValid(it)) {
      LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
      return it->get();
    }
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    return nullptr;
  }
  void RemoveSpotLight(const Identity& id) {
    auto& cont = GetLayerControllerPtr()->GetSpotLightController();
    if (!cont)
      return;
    cont->Remove(id);
  }
  PointLightFactoryImGui point_light_fact_;
  SpotLightFactoryImGui spot_light_fact_;
  MenuAction menu_action_;
};

std::unique_ptr<LayerControllerUILayer> LayerControllerUILayer::Create() {
  return std::make_unique<LayerControllerUILayerImGUI>();
}
