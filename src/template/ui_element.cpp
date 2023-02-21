#include <MEP-3D/template/ui_element.hpp>

#include <MEP-3D/utils.hpp>

#include <glog/logging.h>

#undef ADD_UI_ELEMENT

namespace mep {
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

ElementData& ElementData::Get() {
  static ElementData instance;
  return instance;
}

void ElementData::CreateElement(int id, const std::string& name) {
  DCHECK(id > UI_ELEMENT_COUNT);
  DCHECK(Get().elements_.find(name) == Get().elements_.end());
  Get().elements_[name] = id;
}

int ElementData::IdFromString(const std::string& name) {
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
    if (Get().elements_.find(name) != Get().elements_.end()) {
      return Get().elements_[name];
    }
    return Element::Unknown;
  }
}  // namespace UI

std::string ElementData::IdToString(int id) {
  DCHECK(id >= 0);
  if (id < UI_ELEMENT_COUNT) {
    return ToString(static_cast<Element>(id));
  } else {
    for (auto& ele : Get().elements_) {
      if (ele.second == id) {
        return ele.first;
      }
    }
  }
  return "Unknown";
}
};  // namespace UI
}  // namespace mep
