#include "point_light_factory_imgui.hpp"
#include <imgui.h>
#include <MEP-3D/template/util_common_draw.hpp>

PointLightFactoryImGui::PointLightFactoryImGui(
    T::ElementAddCallback add_callback,
    T::ElementRemoveCallback remove_callback)
    : T(add_callback, remove_callback) {}
bool PointLightFactoryImGui::New() {
  AmbientConfig ambient_config = {White, 0.1f};
  PointConfig point_config = {{0.0, 0.0, 0.0}, 0.1, 0.1, 1.0};
  float diffuse_intensity = 1;
  auto pl = std::make_unique<PointLight>(ambient_config, point_config,
                                         diffuse_intensity);
  Update(GetAddCallback()(std::move(pl)));
  return Exists();
}

void PointLightFactoryImGui::ImGUIDraw() {
  if (!Exists()) {
    if (!New())
      return;
  }
  UI::DrawPointLight(*Get());
  if (ImGui::Button("Add")) {
    Update(nullptr);
    ImGui::CloseCurrentPopup();
  }
  ImGui::SameLine();
  if (ImGui::Button("Close")) {
    Remove();
    ImGui::CloseCurrentPopup();
  }
}