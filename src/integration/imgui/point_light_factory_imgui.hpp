#ifndef POINT_lIGHT_FACTORY_IMGUI_HPP
#define POINT_lIGHT_FACTORY_IMGUI_HPP

#include <MEP-3D/light_controller.hpp>
#include "element_factory_imgui.hpp"

class PointLightFactoryImGui
    : public ElementFactoryImGui<PointLight, PointLightPtr> {
 public:
  using T = ElementFactoryImGui<PointLight, PointLightPtr>;
  PointLightFactoryImGui(T::ElementAddCallback add_callback,
                         T::ElementRemoveCallback remove_callback);
  bool New(PointLightPtr element) override;
  void ImGUIDraw(Scene& scene) override;
};

#endif