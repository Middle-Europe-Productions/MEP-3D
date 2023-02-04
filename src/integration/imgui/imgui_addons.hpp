#ifndef IMGUI_ADDONS_HPP
#define IMGUI_ADDONS_HPP

namespace ImGui {
bool Spinner(const char* label,
             float radius,
             int thickness,
             const unsigned int& color);
void ApplyMepSceneStyle();
void ApplyMepStyle();
}  // namespace ImGui

#endif