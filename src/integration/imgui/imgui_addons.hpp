#ifndef IMGUI_ADDONS_HPP
#define IMGUI_ADDONS_HPP

#include <imgui.h>

namespace ImGui {
bool Spinner(const char* label,
             float radius,
             int thickness,
             const unsigned int& color);
bool BufferingBar(const char* label,
                  float value,
                  const ImVec2& size_arg,
                  const ImU32& bg_col,
                  const ImU32& fg_col);
void ApplyMepSceneStyle();
void ApplyMepStyle();
}  // namespace ImGui

#endif