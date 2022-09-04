
#include "scene_ui_parser_imgui.hpp"
#include <glog/logging.h>
#include <imgui.h>

SceneUIParserImGui::SceneUIParserImGui() : SceneUIParser() {}

void SceneUIParserImGui::Parse(const std::string& json) {
  try {
    root_ = nlohmann::json::parse(json);
  } catch (const std::exception& e) {
    LOG(FATAL) << "Could not parse! Error: " << e.what();
    return;
  }
  for (auto& [key, value] : root_.items()) {
    if (key == "menu") {
      if (value.is_array()) {
        head_ = new SceneUIParserNode();
        head_->start_callback = std::bind(&ImGui::BeginMenuBar);
        head_->finish_callback = std::bind(&ImGui::EndMenuBar);
        for (auto& ele : value) {
          head_->next.push_back(ParseMenuItem(ele));
        }
      }
    } else {
      LOG(INFO) << "Unknown parse key " << key << ", value " << value
                << ", ignoring tree!";
    }
  }
}

SceneUIParser::SceneUIParserNode* SceneUIParserImGui::ParseMenuItem(
    nlohmann::json& json_data) {
  SceneUIParserNode* node = new SceneUIParserNode();
  for (auto& [key, value] : json_data.items()) {
    if (key == "return" && value.is_array()) {
      for (auto& ele : value) {
        node->next.push_back(ParseMenuItem(ele));
      }
    } else if (key == "return" && value.is_number()) {
      node->return_code = value;
    } else if (key == "name") {
      node->node_name = value;
    } else {
      LOG(ERROR) << "Invalid item: " << key;
      delete node;
      return nullptr;
    }
  }
  if (node->next.size() > 0) {
    LOG(INFO) << "Parsing BeginMenu with name " << node->node_name;
    node->start_callback =
        std::bind(static_cast<bool (&)(const char*, bool)>(ImGui::BeginMenu),
                  node->node_name.c_str(), true);
    node->finish_callback = std::bind(&ImGui::EndMenu);
  } else {
    LOG(INFO) << "Parsing MenuItem with name " << node->node_name
              << ", return code " << node->return_code;
    node->start_callback =
        std::bind(static_cast<bool (&)(const char*, const char*, bool, bool)>(
                      ImGui::MenuItem),
                  node->node_name.c_str(), (const char*)0, false, true);
  }
  LOG(INFO) << "Elements parsed " << node->next.size();
  return node;
}
