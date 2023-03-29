
#include "parser_imgui.hpp"
#include <glog/logging.h>
#include <imgui.h>

#include <MEP-3D/common_names.hpp>
#include <MEP-3D/template/ui_element.hpp>
#include <MEP-3D/utils.hpp>

namespace mep {
namespace {
constexpr char kTabBarId[] = "##tab_bar";
}

ImguiPopupController& ImguiPopupController::Get() {
  static ImguiPopupController instance;
  return instance;
}

void ImguiPopupController::RequestPopupOpen(const std::string& name) {
  ImguiPopupController::Get().requested_popups_.push(name);
}

bool ImguiPopupController::OpenAllOnCurrentStack() {
  auto& elements = ImguiPopupController::Get().requested_popups_;
  while (!elements.empty()) {
    ImGui::OpenPopup(elements.back().c_str());
    elements.pop();
  }
  return true;
}

ParserImGui::ParserImGui() : Parser() {}

void ParserImGui::Parse(const std::string& json) {
  VLOG(3) << __func__;
  try {
    root_ = nlohmann::json::parse(json);
  } catch (const std::exception& e) {
    LOG(FATAL) << "Could not parse! Error: " << e.what();
    return;
  }
  for (auto& [key, value] : root_.items()) {
    if (key == std::string(kWindowNodeName)) {
      DCHECK(value.is_array())
          << "Invalid window node, list of elements is expected";
      for (auto& window : value) {
        auto* node = new ParserNode();
        for (auto& [sub_key, sub_value] : window.items()) {
          if (sub_key == "name") {
            VLOG(3) << "Detected key \"name\":" << sub_value;
            node->node_name = sub_value;
          } else if (sub_key == "return") {
            VLOG(3) << "Detected key \"return\"";
            node->next = ParseWindow(sub_value);
          } else if (sub_key == "file") {
            VLOG(3) << "Detected key \"file\"";
            auto sub_config = utils::LoadFromFile(sub_value);
            DCHECK(sub_config != "");
            try {
              auto new_parse_value = nlohmann::json::parse(sub_config);
              node->next = ParseWindow(new_parse_value);
            } catch (const std::exception& e) {
              LOG(FATAL) << "Could not parse! Error: " << e.what();
              continue;
            }
          } else {
            LOG(WARNING) << "Unkown item, key: " << sub_key
                         << ", value: " << sub_value;
          }
        }
        node->start_callback = std::bind(ImGui::Begin, node->node_name.c_str(),
                                         (bool*)NULL, ImGuiWindowFlags_MenuBar);
        node->finish_callback = std::bind(ImGui::End);
        windows_root_.push_back(node);
      }
    }
  }
}

std::vector<Parser::ParserNode*> ParserImGui::ParseWindow(
    nlohmann::json& json_data) {
  VLOG(3) << __func__;
  std::vector<ParserNode*> nodes;
  for (auto& [key, value] : json_data.items()) {
    auto ele = ElementFromString(key);
    if (ele == Element::Count) {
      LOG(ERROR) << "Unknown parse key: " << key << ", ignoring tree!";
      VLOG(1) << "Value: " << value;
      continue;
    }
    VLOG(2) << "Parsing: " << key << ", value: " << static_cast<int>(ele);
    if (ele == Element::Menu) {
      if (value.is_array()) {
        auto* node = new ParserNode();
        node->start_callback = std::bind(&ImGui::BeginMenuBar);
        node->finish_callback = std::bind(&ImGui::EndMenuBar);
        for (auto& ele : value) {
          node->next.push_back(ParseMenuItem(ele));
        }
        nodes.push_back(node);
      } else {
        LOG(WARNING) << "Value is not an array";
      }
    } else if (ele == Element::Scene) {
      if (value.is_array()) {
        auto* node = new ParserNode();
        node->start_callback =
            std::bind(ImGui::BeginTabBar, kTabBarId, ImGuiTabBarFlags_None);
        node->finish_callback = std::bind(ImGui::EndTabBar);
        for (auto& ele : value) {
          node->next.push_back(ParseSceneItem(ele, 0));
        }
        nodes.push_back(node);
      } else {
        LOG(WARNING) << "Value is not an array";
      }
    } else if (ele == Element::Popup) {
      if (value.is_array()) {
        auto* node = new ParserNode();
        node->start_callback =
            std::bind(&ImguiPopupController::OpenAllOnCurrentStack,
                      ImguiPopupController::Get());
        for (auto& ele : value) {
          node->next.push_back(ParsePopupItem(ele, 0));
        }
        nodes.push_back(node);
      } else {
        LOG(WARNING) << "Value is not an array";
      }
    }
  }
  return nodes;
}

Parser::ParserNode* ParserImGui::ParseMenuItem(nlohmann::json& json_data) {
  ParserNode* node = new ParserNode();
  for (auto& [key, value] : json_data.items()) {
    if (key == "return" && value.is_array()) {
      for (auto& ele : value) {
        node->next.push_back(ParseMenuItem(ele));
      }
    } else if (key == "return") {
      if (value.is_number()) {
        node->return_code = value;
      } else if (value.is_string()) {
        node->return_code = UI::ElementData::IdFromString(value);
        DCHECK(node->return_code != static_cast<int>(UI::Element::Unknown));
      } else {
        LOG(ERROR) << "Unknown return type";
        delete node;
        return nullptr;
      }
    } else if (key == "name") {
      node->node_name = value;
    } else {
      LOG(ERROR) << "Invalid item: " << key;
      delete node;
      return nullptr;
    }
  }
  if (node->next.size() > 0) {
    VLOG(3) << "Parsing BeginMenu with name " << node->node_name;
    node->start_callback =
        std::bind(static_cast<bool (&)(const char*, bool)>(ImGui::BeginMenu),
                  node->node_name.c_str(), true);
    node->finish_callback = std::bind(&ImGui::EndMenu);
  } else {
    VLOG(3) << "Parsing MenuItem with name " << node->node_name
            << ", return code " << node->return_code;
    node->start_callback =
        std::bind(static_cast<bool (&)(const char*, const char*, bool, bool)>(
                      ImGui::MenuItem),
                  node->node_name.c_str(), (const char*)0, false, true);
  }
  VLOG(3) << "Elements parsed " << node->next.size();
  return node;
}

Parser::ParserNode* ParserImGui::ParseSceneItem(nlohmann::json& json_data,
                                                int depth) {
  ParserNode* node = new ParserNode();
  for (auto& [key, value] : json_data.items()) {
    if (key == "return" && value.is_array()) {
      for (auto& ele : value) {
        node->next.push_back(ParseSceneItem(ele, depth + 1));
      }
    } else if (key == "return") {
      if (value.is_number()) {
        node->return_code = value;
      } else if (value.is_string()) {
        node->return_code = UI::ElementData::IdFromString(value);
        DCHECK(node->return_code != static_cast<int>(UI::Element::Unknown));
      } else {
        LOG(ERROR) << "Unknown return type";
        delete node;
        return nullptr;
      }
    } else if (key == "name") {
      node->node_name = value;
    } else {
      LOG(ERROR) << "Invalid item: " << key;
      delete node;
      return nullptr;
    }
  }
  if (depth == 0) {
    node->start_callback =
        std::bind(ImGui::BeginTabItem, node->node_name.c_str(), (bool*)NULL, 0);
    node->finish_callback = std::bind(ImGui::EndTabItem);
  } else if (depth >= 1) {
    node->start_callback = std::bind(
        static_cast<bool (&)(const char*, int)>(ImGui::CollapsingHeader),
        node->node_name.c_str(), 0);
  }
  return node;
}

Parser::ParserNode* ParserImGui::ParsePopupItem(nlohmann::json& json_data,
                                                int depth) {
  if (depth > 1) {
    LOG(WARNING) << "Invalid scene element depth is " << depth;
    return nullptr;
  }
  ParserNode* node = new ParserNode();
  for (auto& [key, value] : json_data.items()) {
    if (key == "return" && value.is_array()) {
      for (auto& ele : value) {
        node->next.push_back(ParseSceneItem(ele, depth + 1));
      }
    } else if (key == "return") {
      if (value.is_number()) {
        node->return_code = value;
      } else if (value.is_string()) {
        node->return_code = UI::ElementData::IdFromString(value);
        DCHECK(node->return_code != static_cast<int>(UI::Element::Unknown));
      } else {
        LOG(ERROR) << "Unknown return type";
        delete node;
        return nullptr;
      }
    } else if (key == "name") {
      node->node_name = value;
    } else {
      LOG(ERROR) << "Invalid item: " << key;
      delete node;
      return nullptr;
    }
  }
  if (node->node_name.empty()) {
    node->node_name = UI::ElementData::IdToString(node->return_code);
  }
  node->start_callback = std::bind(ImGui::BeginPopup, node->node_name.c_str(),
                                   ImGuiWindowFlags_MenuBar);
  node->finish_callback = std::bind(ImGui::EndPopup);

  return node;
}
}  // namespace mep
