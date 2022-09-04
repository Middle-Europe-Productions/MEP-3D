#ifndef ELEMENT_FACTORY_IMGUI_HPP
#define ELEMENT_FACTORY_IMGUI_HPP

#include <MEP-3D/scene.hpp>
#include <functional>

class ElementFactoryImGuiBase {
 public:
  virtual void Remove() = 0;
  virtual void ImGUIDraw(Scene&) = 0;
  virtual ~ElementFactoryImGuiBase() = default;
};

template <typename Element, typename ElementPtr>
class ElementFactoryImGui : public ElementFactoryImGuiBase {
 public:
  using ElementAddCallback = std::function<Element*(ElementPtr)>;
  using ElementRemoveCallback = std::function<void(const Identity&)>;
  ElementFactoryImGui(ElementAddCallback add_callback,
                      ElementRemoveCallback remove_callback);
  void Update(Element* element);
  bool Exists() const;
  Element* Get();
  ElementAddCallback GetAddCallback() { return add_callback_; }
  ElementRemoveCallback GetRemoveCallback() { return remove_callback_; }
  virtual void Remove();
  virtual bool New(ElementPtr element) = 0;
  virtual ~ElementFactoryImGui() = default;

 private:
  ElementAddCallback add_callback_;
  ElementRemoveCallback remove_callback_;
  Element* element_;
};

#include "element_factory_imgui.inl"

#endif