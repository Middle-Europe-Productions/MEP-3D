#include <MEP-3D/template/ui_element.hpp>

#include <MEP-3D/utils.hpp>

namespace UI {
std::string ToString(Element element) {
  switch (element) {
    case Element::Unknown:
      return "Unknown";
    case Element::None:
      return "None";
#define ADD_UI_ELEMENT(var_name, var_string) \
  case var_name:                             \
    return var_string;
#include <MEP-3D/template/ui_elements.inl>
#undef ADD_UI_ELEMENT
    default:
      NOTREACHED();
  }
}

int ElementData::GetElementCount() {
  return static_cast<int>(Element::Count);
}

int ElementData::GetElementId(Element element) {
  return static_cast<int>(element);
}

int ElementData::GetAvalibleId() {
  return GetElementId(Element::Count) + 1;
}

Element ElementData::ElementFromString(const std::string& name) {
  if (name.compare("None") == 0) {
    return Element::None;
  }
#define ADD_UI_ELEMENT(var_name, var_string) \
  else if (name.compare(var_string) == 0) {  \
    return Element::var_name;                \
  }
#include <MEP-3D/template/ui_elements.inl>
#undef ADD_UI_ELEMENT
  else {
    return Element::Unknown;
  }
}  // namespace UI
};  // namespace UI
