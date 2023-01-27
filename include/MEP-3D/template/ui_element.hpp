#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include <MEP-3D/scene_ui_parser.hpp>

#include <string>

namespace UI {

enum Element {
  Unknown = -2,
  None = -1,
#define ADD_UI_ELEMENT(var_name, var_string) var_name,
#include <MEP-3D/template/scene_ui_layer_elements.inl>
#undef ADD_UI_ELEMENT
  Count
};

std::string ToString(Element element);

class ElementData {
 public:
  static int GetElementCount();
  static int GetElementId(Element element);
  static Element ElementFromString(const std::string& name);
};

};  // namespace UI

#endif