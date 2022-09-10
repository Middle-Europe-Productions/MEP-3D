#ifndef MODEL_FACTORY_HPP
#define MODEL_FACTORY_HPP

#include <MEP-3D/model.hpp>
#include "element_factory_imgui.hpp"

class ModelFactoryImGui : public ElementFactoryImGui<Model, ModelPtr> {
 public:
  using T = ElementFactoryImGui<Model, ModelPtr>;
  ModelFactoryImGui(T::ElementAddCallback add_callback,
                    T::ElementRemoveCallback remove_callback);
  bool New(ModelPtr element) override;
  void ImGUIDraw(Scene& scene) override;

 private:
  bool ValidData() const;
  std::string path_;
  char buffer_name[64];
  char buffer_path[64];
  int selected_shader_;
  int selected_material_;
  int selected_texture_;
};

#endif