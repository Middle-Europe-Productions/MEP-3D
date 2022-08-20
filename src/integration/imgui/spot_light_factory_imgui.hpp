#ifndef SPOT_lIGHT_FACTORY_IMGUI_HPP
#define SPOT_lIGHT_FACTORY_IMGUI_HPP

#include <MEP-3D/light_controller.hpp>
#include "element_factory_imgui.hpp"

class SpotLightFactoryImGui
    : public ElementFactoryImGui<SpotLight, SpotLightPtr> {
 public:
  using T = ElementFactoryImGui<SpotLight, SpotLightPtr>;
  SpotLightFactoryImGui(T::ElementAddCallback add_callback,
                        T::ElementRemoveCallback remove_callback);
  bool New() override;
  void ImGUIDraw() override;
};

#endif