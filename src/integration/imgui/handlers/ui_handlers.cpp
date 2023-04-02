#include <MEP-3D/template/scene_ui_layer_handler_context.hpp>

#include <MEP-3D/platform/platform_delegate.hpp>
#include <MEP-3D/template/ui_element.hpp>
#include <MEP-3D/template/util_common_draw.hpp>

#include "../enum_widget.hpp"
#include "resource_wrap.hpp"
#include "shader_editor_context.hpp"
#include "volume_wrap.hpp"

#include <imgui.h>

namespace mep {
namespace {
constexpr auto kInputModelName = "Model name";
const FileFilter kVolumeFileFilter = {"Volume (*.raw)", {{"raw"}}};
const FileFilter kModelFileFilter = {"Model (*.obj)", {{"obj"}}};
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
    if (ImGui::TreeNode(it->get()->Identity::GetUniqueName().c_str())) {
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
    if (ImGui::TreeNode(it->get()->Identity::GetUniqueName().c_str())) {
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
    if (ImGui::TreeNode(pl_ptr->Identity::GetUniqueName().c_str())) {
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
    if (ImGui::TreeNode(dl_ptr->Identity::GetUniqueName().c_str())) {
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
    if (ImGui::TreeNode(sl_ptr->Identity::GetUniqueName().c_str())) {
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
    if (ImGui::TreeNode(sh_ptr->Identity::GetUniqueName().c_str())) {
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
    if (ImGui::TreeNode(pc->Identity::GetUniqueName().c_str())) {
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

struct PointLightWrap : public UI_HANDLER_BASE(SceneUILayer) {
  PointLight* point_light = nullptr;
};

UI_HANDLER_D(UI::Element, PointLightPopup, SceneUILayer, PointLightWrap) {
  if (!point_light) {
    auto& cont = GetContext()->GetScenePtr()->GetPointLightController();
    auto it = cont->MakeAndBind(PointLight::Create());
    DCHECK(cont->IsValid(it));
    point_light = it->get();
  }
  UI::Drawer::DrawPointLight(*point_light);
  if (ImGui::Button("Add")) {
    point_light = nullptr;
    ImGui::CloseCurrentPopup();
  }
  ImGui::SameLine();
  if (ImGui::Button("Cancel")) {
    DCHECK(point_light);
    auto& cont = GetContext()->GetScenePtr()->GetPointLightController();
    if (!cont)
      return;
    cont->Remove(*point_light);
    point_light = nullptr;
    ImGui::CloseCurrentPopup();
  }
}

UI_HANDLER(UI::Element, OpenSpotLightPopup, SceneUILayer) {
  LOG(INFO) << "Opening popup " << ToString(UI::Element::SpotLightPopup);
  UI::Drawer::OpenPopup(UI::Element::SpotLightPopup);
}

struct SpotLightWrap : public UI_HANDLER_BASE(SceneUILayer) {
  SpotLight* spot_light = nullptr;
};

UI_HANDLER_D(UI::Element, SpotLightPopup, SceneUILayer, SpotLightWrap) {
  if (!spot_light) {
    auto& cont = GetContext()->GetScenePtr()->GetSpotLightController();
    auto it = cont->MakeAndBind(SpotLight::Create());
    DCHECK(cont->IsValid(it));
    spot_light = it->get();
  }
  UI::Drawer::DrawSpotLight(*spot_light);
  if (ImGui::Button("Add")) {
    spot_light = nullptr;
    ImGui::CloseCurrentPopup();
  }
  ImGui::SameLine();
  if (ImGui::Button("Cancel")) {
    DCHECK(spot_light);
    auto& cont = GetContext()->GetScenePtr()->GetSpotLightController();
    if (!cont)
      return;
    cont->Remove(*spot_light);
    spot_light = nullptr;
    ImGui::CloseCurrentPopup();
  }
}

UI_HANDLER(UI::Element, OpenModelPopup, SceneUILayer) {
  LOG(INFO) << "Opening popup " << ToString(UI::Element::ModelPopup);
  UI::Drawer::OpenPopup(UI::Element::ModelPopup);
}

UI_HANDLER_D(UI::Element, ModelPopup, SceneUILayer, ResourceWrap) {
  ImGui::InputTextWithHint("##model_name", kInputModelName, display_name,
                           kMaxPathSize, ImGuiInputTextFlags_CharsNoBlank);
  if (ImGui::Button("Open File")) {
    std::filesystem::path file_path = PlatformDelegate::Get()->OpenFile(
        GetContext()->GetEngine()->GetWindow().get(), kModelFileFilter);
    if (!file_path.empty()) {
      LOG(INFO) << "File path " << file_path;
      buffer_path = file_path;
    }
  }
  ImGui::Text((const char*)buffer_path.generic_u8string().c_str(),
              ImGuiInputTextFlags_CharsNoBlank);
  DrawSelectable(GetContext());
  ImGui::Separator();
  if (!ValidData()) {
    ImGui::BeginDisabled();
  }
  if (ImGui::Button("Load")) {
    auto model = std::make_unique<Model>(std::string(display_name));
    EvalSelectable(model.get(), GetContext());
    LOG(WARNING) << "Model class does not support utf-16";
    model->Load(buffer_path.string());
    GetContext()->GetScenePtr()->GetModels().emplace_back(std::move(model));
    ImGui::CloseCurrentPopup();
  }
  ImGui::SameLine();
  if (!ValidData()) {
    ImGui::EndDisabled();
  }
  if (ImGui::Button("Cancel")) {
    ImGui::CloseCurrentPopup();
  }
}

UI_HANDLER_D(UI::Element, VolumePopup, SceneUILayer, VolumeWrap) {
  ImGui::InputTextWithHint("##volume_name", kInputModelName, display_name,
                           kMaxPathSize, ImGuiInputTextFlags_CharsNoBlank);
  Init(GetContext(), kVolumeFileFilter);
  ImGui::Text((const char*)buffer_path.filename().generic_u8string().c_str(),
              ImGuiInputTextFlags_CharsNoBlank);
  DrawSelectable(GetContext());
  ImGui::Text("Volume size");
  UI::Drawer::Draw(size, 0, 10240, "##volume_size");
  ImGui::Text("Data type");
  enum_widget.Draw();
  ImGui::Separator();
  if (!ValidData()) {
    ImGui::BeginDisabled();
  }
  if (ImGui::Button("Load")) {
    GetContext()->GetScenePtr()->GetVolume().emplace_back(Create(GetContext()));
    Uninitialize();
    ImGui::CloseCurrentPopup();
  }
  if (!ValidData()) {
    ImGui::EndDisabled();
  }
  ImGui::SameLine();
  if (ImGui::Button("Open File")) {
    SetFilePath(PlatformDelegate::Get()->OpenFile(
        GetContext()->GetEngine()->GetWindow().get(), kVolumeFileFilter));
  }
  ImGui::SameLine();
  if (ImGui::Button("Cancel")) {
    Uninitialize();
    ImGui::CloseCurrentPopup();
  }
}

UI_HANDLER(UI::Element, OpenVolumePopup, SceneUILayer) {
  LOG(INFO) << "Opening popup " << ToString(UI::Element::VolumePopup);
  UI::Drawer::OpenPopup(UI::Element::VolumePopup);
}

UI_HANDLER(UI::Element, SystemInfo, SceneUILayer) {
  constexpr std::size_t kDivider = 1024;
  PlatformDelegate::MemorySnapshot memory_snapsot =
      PlatformDelegate::Get()->GetMemorySnapshot();
  ImGui::Text("RAM Avalible: %li MB",
              memory_snapsot.avalible_ram_memory_kb / kDivider);
  ImGui::Text("RAM Total: %li MB",
              memory_snapsot.total_ram_memory_kb / kDivider);
  ImGui::Separator();
  ImGui::Text("CPU");
  ImGui::Text("Info: %s", memory_snapsot.cpu_info.c_str());
  ImGui::Text("Type: %s", ToString(memory_snapsot.processor_type).c_str());
  ImGui::Text("Cores: %lu", memory_snapsot.number_of_avalible_cores);
  ImGui::Separator();
  ImGui::Text("GPU");
  ImGui::Text("Vendor: %s", memory_snapsot.gpu_vendor.c_str());
  ImGui::Text("Name: %s", memory_snapsot.gpu_name.c_str());
  ImGui::Text("Driver: %s", memory_snapsot.gpu_driver.c_str());
}

UI_HANDLER_D(UI::Element, ShaderEditor, SceneUILayer, ShaderEditorContext) {
  auto& shaders = GetContext()->GetScenePtr()->GetShaders();
  SelectShader(UI::Drawer::DrawShaderComboMenu(shaders, selected_shader_),
               shaders);
  DrawUtilityMenu();
  DrawShader();
}

}  // namespace mep
