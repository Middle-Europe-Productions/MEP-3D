template <typename Element, typename ElementPtr>
ElementFactoryImGui<Element, ElementPtr>::ElementFactoryImGui(
    ElementAddCallback add_callback,
    ElementRemoveCallback remove_callback)
    : add_callback_(add_callback),
      remove_callback_(remove_callback),
      element_(nullptr) {}

template <typename Element, typename ElementPtr>
void ElementFactoryImGui<Element, ElementPtr>::Update(Element* element) {
  element_ = element;
}

template <typename Element, typename ElementPtr>
bool ElementFactoryImGui<Element, ElementPtr>::Exists() const {
  return element_ != nullptr;
}

template <typename Element, typename ElementPtr>
Element* ElementFactoryImGui<Element, ElementPtr>::Get() {
  return element_;
}
