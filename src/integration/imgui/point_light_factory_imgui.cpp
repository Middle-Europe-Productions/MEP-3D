#include "point_light_factory_imgui.hpp"
#include <imgui.h>
#include <MEP-3D/template/util_common_draw.hpp>

PointLightFactoryImGui::PointLightFactoryImGui(
    T::ElementAddCallback add_callback,
    T::ElementRemoveCallback remove_callback)
    : T(add_callback, remove_callback) {}
bool PointLightFactoryImGui::New(PointLightPtr element) {
  LOG(INFO) << __FUNCTION__;
  Update(GetAddCallback()(std::move(element)));
  return Exists();
}

void PointLightFactoryImGui::ImGUIDraw(Scene& scene) {
  if (!Exists()) {
    if (!New(PointLight::Create()))
      return;
  }
  UI::DrawPointLight(*Get());
  if (ImGui::Button("Add")) {
    Update(nullptr);
    ImGui::CloseCurrentPopup();
  }
  ImGui::SameLine();
  if (ImGui::Button("Cancel")) {
    Remove();
    ImGui::CloseCurrentPopup();
  }
}