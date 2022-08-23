#include <imgui.h>
#include <functional>

#include <MEP-3D/template/layer_controller_ui_layer.hpp>
#include <MEP-3D/template/util_common_draw.hpp>

#include "model_factory_imgui.hpp"
#include "point_light_factory_imgui.hpp"
#include "spot_light_factory_imgui.hpp"

namespace {
enum MenuAction { None, AddPointLight, AddSpotLight, AddModel };
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
                      std::placeholders::_1)),
        model_fact_(std::bind(&LayerControllerUILayerImGUI::AddModel,
                              this,
                              std::placeholders::_1),
                    std::bind(&LayerControllerUILayerImGUI::RemoveModel,
                              this,
                              std::placeholders::_1)) {
    LOG(INFO) << __FUNCTION__;
  }
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
        if (ImGui::MenuItem("Model")) {
          menu_action_ = MenuAction::AddModel;
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
      ImGui::Separator();
      UI::DrawAssetControllerConst(
          *GetLayerControllerPtr()->GetSpotLightController());
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
        ImGui::Separator();
        UI::DrawAssetControllerConst(
            *GetLayerControllerPtr()->GetSpotLightController());
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
        ImGui::Separator();
        UI::DrawAssetControllerConst(
            *GetLayerControllerPtr()->GetSpotLightController());
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
    for (auto& model : GetLayerControllerPtr()->GetModels()) {
      if (ImGui::TreeNode(model->Identity::GetName().c_str())) {
        UI::DrawModel(*model.get());
        ImGui::Separator();
        ImGui::TreePop();
      }
    }
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
      case MenuAction::AddModel:
        ImGui::OpenPopup("New Model");
        menu_action_ = MenuAction::None;
        break;
    };
    DrawFactory("New Point Light", point_light_fact_);
    DrawFactory("New Spot Light", spot_light_fact_);
    DrawFactory("New Model", model_fact_);
    DrawMenu();
    ImGui::End();
  }
  ~LayerControllerUILayerImGUI() { LOG(INFO) << __FUNCTION__; }

 private:
  template <typename What>
  void DrawFactory(const std::string& name, What& factory) {
    if (ImGui::BeginPopup(name.c_str(), ImGuiWindowFlags_MenuBar)) {
      factory.ImGUIDraw(*GetLayerControllerPtr());
      ImGui::EndPopup();
    } else {
      factory.Remove();
    }
  }
  PointLight* AddPointLight(PointLightPtr point_light) {
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    auto& cont = GetLayerControllerPtr()->GetPointLightController();
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
    auto& cont = GetLayerControllerPtr()->GetSpotLightController();
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
    GetLayerControllerPtr()->GetModels().emplace_back(std::move(model));
    return GetLayerControllerPtr()->GetModels().back().get();
  }
  void RemovePointLight(const Identity& id) {
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    auto& cont = GetLayerControllerPtr()->GetPointLightController();
    if (!cont)
      return;
    cont->Remove(id);
  }
  void RemoveSpotLight(const Identity& id) {
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    auto& cont = GetLayerControllerPtr()->GetSpotLightController();
    if (!cont)
      return;
    cont->Remove(id);
  }
  void RemoveModel(const Identity& id) {
    LOG(INFO) << __FUNCTION__ << ", " << __LINE__;
    auto& cont = GetLayerControllerPtr()->GetModels();
    cont.erase(std::remove_if(cont.begin(), cont.end(),
                              [&id](const auto& in) {
                                return in->GetGlobalId() == id.GetGlobalId();
                              }),
               cont.end());
  }
  virtual bool ShouldIgnoreLayer() const override { return false; }
  PointLightFactoryImGui point_light_fact_;
  SpotLightFactoryImGui spot_light_fact_;
  ModelFactoryImGui model_fact_;
  MenuAction menu_action_;
};

std::unique_ptr<LayerControllerUILayer> LayerControllerUILayer::Create() {
  return std::make_unique<LayerControllerUILayerImGUI>();
}
