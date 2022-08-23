#include "model_factory_imgui.hpp"

#include <imgui.h>

#include <MEP-3D/model.hpp>
#include <MEP-3D/template/util_common_draw.hpp>

namespace {
constexpr auto kInputModelName = "Model name";
constexpr auto kInputPathName = "File path";
};  // namespace

ModelFactoryImGui::ModelFactoryImGui(T::ElementAddCallback add_callback,
                                     T::ElementRemoveCallback remove_callback)
    : T(add_callback, remove_callback),
      buffer_name(""),
      buffer_path(""),
      selected_shader_(-1) {}
bool ModelFactoryImGui::New(ModelPtr element) {
  Update(GetAddCallback()(std::move(element)));
  return Exists();
}
void ModelFactoryImGui::ImGUIDraw(LayerController& layer_controller) {
  ImGui::InputTextWithHint("##model_name", kInputModelName, buffer_name, 64,
                           ImGuiInputTextFlags_CharsNoBlank);
  ImGui::InputTextWithHint("##model_path", kInputPathName, buffer_path, 64,
                           ImGuiInputTextFlags_CharsNoBlank);
  selected_shader_ =
      UI::DrawShaderComboMenu(layer_controller.GetShader(), selected_shader_);
  if (ImGui::Button("Open File")) {
    LOG(INFO) << "Open file";
  }
  ImGui::Separator();
  if (!ValidData()) {
    ImGui::BeginDisabled();
  }
  if (ImGui::Button("Load")) {
    New(std::make_unique<Model>(buffer_name));
    Get()->Load(buffer_path);
    Update(nullptr);
    ImGui::CloseCurrentPopup();
  }
  ImGui::SameLine();
  if (!ValidData()) {
    ImGui::EndDisabled();
  }
  if (ImGui::Button("Cancel")) {
    Remove();
    ImGui::CloseCurrentPopup();
  }
}

bool ModelFactoryImGui::ValidData() const {
  return !(strcmp(buffer_name, "") == 0 || strcmp(buffer_path, "") == 0);
}