#include <MEP-3D/template/scene_ui_layer_handler_context.hpp>

#include <MEP-3D/template/ui_element.hpp>
#include <MEP-3D/template/util_common_draw.hpp>

#include <imgui.h>

namespace {
constexpr auto kInputModelName = "Model name";
constexpr auto kInputPathName = "File path";
};  // namespace

LINK_UI_CONTEXT_AND_HANDLERS(SceneUILayer)

UI_HANDLER(UI::Element, DrawModelMenu, SceneUILayer) {
  auto* context = GetContext();
  DCHECK(context);
  if (context->GetScenePtr()->GetModels().empty()) {
    ImGui::Text("You do not have any models yet!");
    return;
  }
  auto& array = context->GetScenePtr()->GetModels();
  std::queue<std::vector<std::unique_ptr<Model>>::iterator> to_remove;
  for (auto it = array.begin(); it != array.end();) {
    if (ImGui::TreeNode(it->get()->Identity::GetName().c_str())) {
      if (!UI::Drawer::DrawModel(*it->get())) {
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

UI_HANDLER(UI::Element, DrawVolume, SceneUILayer) {
  auto* context = GetContext();
  DCHECK(context);
  if (context->GetScenePtr()->GetVolume().empty()) {
    ImGui::Text("You do not have any volumes yet!");
    return;
  }
  auto& array = context->GetScenePtr()->GetVolume();
  std::queue<std::vector<std::unique_ptr<Volume>>::iterator> to_remove;
  for (auto it = array.begin(); it != array.end();) {
    if (ImGui::TreeNode(it->get()->Identity::GetName().c_str())) {
      if (!UI::Drawer::DrawVolume(*it->get())) {
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
};

UI_HANDLER(UI::Element, DrawPointLight, SceneUILayer) {
  auto* context = GetContext();
  DCHECK(context);
  if (!context->GetScenePtr()->GetPointLightController()) {
    return;
  }
  for (auto& pl_ptr :
       context->GetScenePtr()->GetPointLightController()->GetContainer()) {
    if (ImGui::TreeNode(pl_ptr->Identity::GetName().c_str())) {
      UI::Drawer::DrawPointLight(*pl_ptr.get());
      ImGui::Separator();
      ImGui::TreePop();
    }
  }
  ImGui::Separator();
  UI::Drawer::DrawAssetControllerConst(
      *context->GetScenePtr()->GetPointLightController());
}

UI_HANDLER(UI::Element, DrawDirectionalLight, SceneUILayer) {
  auto* context = GetContext();
  DCHECK(context);
  for (auto& dl_ptr : context->GetScenePtr()->GetDirectionaLights()) {
    if (ImGui::TreeNode(dl_ptr->Identity::GetName().c_str())) {
      UI::Drawer::DrawDirectionalLight(*dl_ptr.get());
      ImGui::Separator();
      ImGui::TreePop();
    }
  }
}

UI_HANDLER(UI::Element, DrawSpotLight, SceneUILayer) {
  auto* context = GetContext();
  DCHECK(context);
  if (context->GetScenePtr()->GetSpotLightController()) {
    return;
  }
  for (auto& sl_ptr :
       context->GetScenePtr()->GetSpotLightController()->GetContainer()) {
    if (ImGui::TreeNode(sl_ptr->Identity::GetName().c_str())) {
      UI::Drawer::DrawSpotLight(*sl_ptr.get());
      ImGui::Separator();
      ImGui::TreePop();
    }
  }
  ImGui::Separator();
  UI::Drawer::DrawAssetControllerConst(
      *context->GetScenePtr()->GetSpotLightController());
}

UI_HANDLER(UI::Element, DrawShader, SceneUILayer) {
  auto* context = GetContext();
  DCHECK(context);
  for (auto& sh_ptr : context->GetScenePtr()->GetShaders()) {
    if (ImGui::TreeNode(sh_ptr->Identity::GetName().c_str())) {
      UI::Drawer::DrawShader(*sh_ptr.get());
      ImGui::Separator();
      ImGui::TreePop();
    }
  }
}

UI_HANDLER(UI::Element, DrawCamera, SceneUILayer) {
  auto* context = GetContext();
  DCHECK(context);
  if (context->GetScenePtr()->GetCamera().empty()) {
    ImGui::Text("You do not have any perspective cameras yet!");
    return;
  }
  for (auto& pc : context->GetScenePtr()->GetCamera()) {
    if (ImGui::TreeNode(pc->Identity::GetName().c_str())) {
      UI::Drawer::DrawCamera(pc.get());
      if (pc.get() == context->GetEngine()->GetWindow()->GetCamera()) {
        ImGui::Text("This camera is currently used");
      }
      ImGui::Separator();
      ImGui::TreePop();
    }
  }
}

UI_HANDLER(UI::Element, DrawWindow, SceneUILayer) {
  auto* context = GetContext();
  DCHECK(context);
  DCHECK(context->GetEngine());
  DCHECK(context->GetEngine()->GetWindow());
  DCHECK(context->GetScenePtr());
  UI::Drawer::DrawWindowInterface(*context->GetEngine()->GetWindow(),
                                  *context->GetScenePtr());
}

UI_HANDLER(UI::Element, None, SceneUILayer) {
  ImGui::Text("Not implemented");
}

UI_HANDLER(UI::Element, OpenPointLightPopup, SceneUILayer) {
  LOG(INFO) << "Opening popup " << ToString(UI::Element::PointLightPopup);
  UI::Drawer::OpenPopup(UI::Element::PointLightPopup);
}

struct PointLightWrap {
  PointLight* point_light = nullptr;
};

UI_HANDLER_D(UI::Element, PointLightPopup, SceneUILayer, PointLightWrap) {
  if (!GetData().point_light) {
    auto& cont = GetContext()->GetScenePtr()->GetPointLightController();
    auto it = cont->MakeAndBind(std::move(PointLight::Create()));
    DCHECK(cont->IsValid(it));
    GetData().point_light = it->get();
  }
  UI::Drawer::DrawPointLight(*GetData().point_light);
  if (ImGui::Button("Add")) {
    GetData().point_light = nullptr;
    ImGui::CloseCurrentPopup();
  }
  ImGui::SameLine();
  if (ImGui::Button("Cancel")) {
    DCHECK(GetData().point_light);
    auto& cont = GetContext()->GetScenePtr()->GetPointLightController();
    if (!cont)
      return;
    cont->Remove(*GetData().point_light);
    GetData().point_light = nullptr;
    ImGui::CloseCurrentPopup();
  }
}

UI_HANDLER(UI::Element, OpenSpotLightPopup, SceneUILayer) {
  LOG(INFO) << "Opening popup " << ToString(UI::Element::SpotLightPopup);
  UI::Drawer::OpenPopup(UI::Element::SpotLightPopup);
}

struct SpotLightWrap {
  SpotLight* spot_light = nullptr;
};

UI_HANDLER_D(UI::Element, SpotLightPopup, SceneUILayer, SpotLightWrap) {
  if (!GetData().spot_light) {
    auto& cont = GetContext()->GetScenePtr()->GetSpotLightController();
    auto it = cont->MakeAndBind(std::move(SpotLight::Create()));
    DCHECK(cont->IsValid(it));
    GetData().spot_light = it->get();
  }
  UI::Drawer::DrawSpotLight(*GetData().spot_light);
  if (ImGui::Button("Add")) {
    GetData().spot_light = nullptr;
    ImGui::CloseCurrentPopup();
  }
  ImGui::SameLine();
  if (ImGui::Button("Cancel")) {
    DCHECK(GetData().spot_light);
    auto& cont = GetContext()->GetScenePtr()->GetSpotLightController();
    if (!cont)
      return;
    cont->Remove(*GetData().spot_light);
    GetData().spot_light = nullptr;
    ImGui::CloseCurrentPopup();
  }
}

UI_HANDLER(UI::Element, OpenModelPopup, SceneUILayer) {
  LOG(INFO) << "Opening popup " << ToString(UI::Element::ModelPopup);
  UI::Drawer::OpenPopup(UI::Element::ModelPopup);
}

struct ModelWrap {
  std::string path_;
  char buffer_name[64];
  char buffer_path[64];
  int selected_shader_ = -1;
  int selected_material_ = -1;
  int selected_texture_ = -1;
  bool ValidData() const {
    return !(strcmp(buffer_name, "") == 0 || strcmp(buffer_path, "") == 0);
  }
};

UI_HANDLER_D(UI::Element, ModelPopup, SceneUILayer, ModelWrap) {
  ImGui::InputTextWithHint("##model_name", kInputModelName,
                           GetData().buffer_name, 64,
                           ImGuiInputTextFlags_CharsNoBlank);
  ImGui::InputTextWithHint("##model_path", kInputPathName,
                           GetData().buffer_path, 64,
                           ImGuiInputTextFlags_CharsNoBlank);
  ImGui::Text("Shader");
  GetData().selected_shader_ = UI::Drawer::DrawShaderComboMenu(
      GetContext()->GetScenePtr()->GetShaders(), GetData().selected_shader_);
  ImGui::Text("Texture");
  GetData().selected_texture_ = UI::Drawer::DrawTextureComboMenu(
      GetContext()->GetScenePtr()->GetTexture(), GetData().selected_texture_);
  ImGui::Text("Material");
  GetData().selected_material_ = UI::Drawer::DrawMaterialComboMenu(
      GetContext()->GetScenePtr()->GetMaterial(), GetData().selected_material_);
  if (ImGui::Button("Open File")) {
    LOG(INFO) << "Open file";
  }
  ImGui::Separator();
  if (!GetData().ValidData()) {
    ImGui::BeginDisabled();
  }
  if (ImGui::Button("Load")) {
    auto model = std::make_unique<Model>(std::string(GetData().buffer_name));
    if (GetData().selected_shader_ != -1) {
      model->Bind(GetContext()
                      ->GetScenePtr()
                      ->GetShaders()[GetData().selected_shader_]
                      .get());
    }
    if (GetData().selected_texture_ != -1) {
      model->Bind(GetContext()
                      ->GetScenePtr()
                      ->GetTexture()[GetData().selected_texture_]
                      .get());
    }
    if (GetData().selected_material_ != -1) {
      model->Bind(GetContext()
                      ->GetScenePtr()
                      ->GetMaterial()[GetData().selected_material_]
                      .get());
    }
    model->Load(GetData().buffer_path);
    GetContext()->GetScenePtr()->GetModels().emplace_back(std::move(model));
    ImGui::CloseCurrentPopup();
  }
  ImGui::SameLine();
  if (!GetData().ValidData()) {
    ImGui::EndDisabled();
  }
  if (ImGui::Button("Cancel")) {
    ImGui::CloseCurrentPopup();
  }
}
