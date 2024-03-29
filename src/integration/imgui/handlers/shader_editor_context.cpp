#include "shader_editor_context.hpp"

#include <regex>

namespace {
constexpr auto kGLSLErrorRegex = "0\\((\\d+)\\)\\s*:\\s*(.+)";
};  // namespace

namespace mep {
void ShaderEditorContext::SelectShader(
    int selected_shader,
    std::vector<std::unique_ptr<Shader>>& shaders) {
  if (selected_shader == selected_shader_) {
    return;
  }
  if (selected_shader < 0 ||
      selected_shader >= static_cast<int>(shaders.size())) {
    should_draw_fragment = false;
    should_draw_vertex = false;
    shader_ = nullptr;
    selected_shader_ = selected_shader;
    return;
  }
  editor_[0].Delete();
  editor_[0].Delete();
  selected_shader_ = selected_shader;
  shader_ = shaders[selected_shader_].get();
  auto& shader_status = shader_->GetShaderStatus();
  should_draw_fragment = shader_status.vertex_code.has_value();
  if (should_draw_fragment) {
    editor_[0].SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
    editor_[0].SetText(shader_status.vertex_code.value());
    editor_[0].SetShowWhitespaces(false);
  }

  should_draw_vertex = shader_status.fragment_code.has_value();
  if (should_draw_vertex) {
    editor_[1].SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
    editor_[1].SetText(shader_status.fragment_code.value());
    editor_[1].SetShowWhitespaces(false);
  }
  LOG(INFO) << __func__ << ", " << __LINE__;
}
void ShaderEditorContext::DrawShader() {
  if (!should_draw_fragment && !should_draw_vertex) {
    ImGui::Text("Please select shader");
    return;
  }
  ImGuiTabBarFlags tab_bar_flags =
      ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_Reorderable;
  if (ImGui::BeginTabBar("Type", tab_bar_flags)) {
    if (should_draw_fragment &&
        ImGui::BeginTabItem("Vertex", nullptr,
                            vertex_changed_ ? ImGuiTabItemFlags_UnsavedDocument
                                            : ImGuiTabItemFlags_None)) {
      if (eval_vertex_change && editor_[0].IsTextChanged()) {
        vertex_changed_ = true;
      }
      editor_[0].Render("Vertex");
      eval_vertex_change = true;
      ImGui::EndTabItem();
    }
    if (should_draw_vertex &&
        ImGui::BeginTabItem("Fragment", nullptr,
                            fragment_changed_
                                ? ImGuiTabItemFlags_UnsavedDocument
                                : ImGuiTabItemFlags_None)) {
      if (eval_fragment_change && editor_[1].IsTextChanged()) {
        fragment_changed_ = true;
      }
      editor_[1].Render("Fragment");
      eval_fragment_change = true;
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
}
void ShaderEditorContext::DrawUtilityMenu() {
  if (!shader_) {
    return;
  }
  if (last_error.error_occured) {
    ImGui::Text("%s", last_error.error_message.c_str());
  }
  if (ImGui::Button("Compile")) {
    if (!Compile()) {
      LOG(WARNING) << "Compilation failed!";
    }
  }
  const auto& status = shader_->GetShaderStatus();
  if (status.created_from_file) {
    ImGui::SameLine();
    if (ImGui::Button("Save")) {
      if (vertex_changed_ || fragment_changed_) {
        if (!Compile()) {
          LOG(ERROR) << "Unable to save uncompiled shader";
          return;
        }
      }
      if (vertex_changed_) {
        LOG(INFO) << "Saving vertex shader";
        utils::SaveToFile(status.vertex_path.value_or(""),
                          status.vertex_code.value_or(""));
        vertex_changed_ = false;
      }
      if (fragment_changed_) {
        LOG(INFO) << "Saving fragment shader";
        utils::SaveToFile(status.fragment_path.value_or(""),
                          status.fragment_code.value_or(""));
        fragment_changed_ = false;
      }
    }
  }
}

bool ShaderEditorContext::Compile() {
  last_error = shader_->ReCompile(
      editor_[0].GetText(), should_draw_fragment ? editor_[1].GetText() : "");
  if (last_error.error_occured) {
    if (last_error.fragment_error != "") {
      VLOG(3) << "Error occured in fragment shader";
      TranslateErrorToMap(fragment_markers, last_error.fragment_error);
    }
    if (last_error.vertex_error != "") {
      VLOG(3) << "Error occured in vertex shader";
      TranslateErrorToMap(vertex_markers, last_error.vertex_error);
    }
  } else {
    fragment_markers.clear();
    vertex_markers.clear();
  }
  UpdateMarkers();
  return !last_error.error_occured;
}

void ShaderEditorContext::UpdateMarkers() {
  editor_[0].SetErrorMarkers(vertex_markers);
  editor_[1].SetErrorMarkers(fragment_markers);
}

void ShaderEditorContext::TranslateErrorToMap(
    TextEditor::ErrorMarkers& error_marker,
    const std::string& message) {
  VLOG(5) << __func__ << ", " << message;
  error_marker.clear();
  std::regex text_regex(kGLSLErrorRegex);
  auto words_begin =
      std::sregex_iterator(message.begin(), message.end(), text_regex);
  auto words_end = std::sregex_iterator();
  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    std::smatch match = *i;
    int position = 0;
    int line = 0;
    std::string message;
    VLOG(5) << "Match found " << match.begin()->str();
    for (auto x = match.begin() + 1; x != match.end(); x++) {
      if (position == 0) {
        line = std::stoi(x->str());
      } else if (position == 1) {
        message = x->str();
      } else {
        LOG(WARNING) << __func__ << ", invalid regex size";
        break;
      }
      position++;
    }
    auto marker_it = error_marker.find(line);
    if (marker_it != error_marker.end()) {
      marker_it->second += "\n" + message;
    } else {
      error_marker.insert({line, message});
    }
  }
}
};  // namespace mep