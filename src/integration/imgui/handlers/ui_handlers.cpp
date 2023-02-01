#include <MEP-3D/template/scene_ui_layer_handler_context.hpp>

#include <MEP-3D/template/ui_element.hpp>
#include <MEP-3D/template/util_common_draw.hpp>

#include <imgui.h>

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
