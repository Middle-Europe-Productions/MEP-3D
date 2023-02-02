#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include <MEP-3D/non_copyable.hpp>
#include <MEP-3D/scene_ui_parser.hpp>

#include <string>

namespace UI {

enum Element {
  Unknown = -2,
  None = -1,
#define ADD_UI_ELEMENT(var_name, var_string) var_name,
#include <MEP-3D/template/ui_elements.inl>
#undef ADD_UI_ELEMENT
  Count
};

#define UI_ELEMENT_COUNT static_cast<int>(UI::Element::Count)

std::string ToString(Element element);

class ElementData : public NonCopyable {
 public:
  static int GetElementCount();
  static int GetElementId(Element element);
  static int GetAvalibleId();
  static int IdFromString(const std::string& name);
  static std::string IdToString(int name);
  static ElementData& Get();
  void CreateElement(int id, const std::string& name);

 private:
  std::unordered_map<std::string, int> elements_;
};

#define ADD_UI_ELEMENT(var_name, var_string) \
  UI::ElementData::Get().CreateElement(static_cast<int>(var_name), var_string)

};  // namespace UI

#endif