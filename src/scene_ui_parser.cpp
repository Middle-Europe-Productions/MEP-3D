#include <glog/logging.h>
#include <MEP-3D/common_names.hpp>
#include <MEP-3D/scene_ui_parser.hpp>
#include <MEP-3D/utils.hpp>

namespace {
constexpr auto DoNothing = []() {};
constexpr auto DoNothingWithReplay = []() -> bool { return false; };
};  // namespace

SceneUIParser::SceneUIParserNode::SceneUIParserNode()
    : start_callback(DoNothingWithReplay),
      finish_callback(DoNothing),
      node_name(""),
      return_code(-1) {}

SceneUIParser::SceneUIParser() : handler_map_() {
  Init();
}

void SceneUIParser::SetHandler(
    const std::unordered_map<int, Callback>& handler_map) {
  handler_map_ = handler_map;
}

void SceneUIParser::MergeHandler(
    const std::unordered_map<int, Callback>& handler_map,
    Method method) {
  for (auto& node : handler_map) {
    auto it = handler_map_.find(node.first);
    if (it != handler_map_.end()) {
      if (utils::Contains(method, Method::Override)) {
        it->second = node.second;
      }
    } else {
      if (utils::Contains(method, Method::FillMissing)) {
        handler_map_[node.first] = node.second;
      }
    }
  }
}

void SceneUIParser::Draw() {
  for (Element it = Element::Menu; it != Element::Count;
       it = utils::IncEnum(it)) {
    DrawRecursive(nodes_[it]);
  }
}

void SceneUIParser::DrawRecursive(SceneUIParserNode* current) {
  if (!current) {
    return;
  }
  if (current->start_callback()) {
    if (current->next.size() == 0) {
      auto callback = handler_map_.find(current->return_code);
      if (callback == handler_map_.end()) {
        LOG(ERROR) << "Unknown handler for return value: "
                   << current->return_code
                   << ", in node: " << current->node_name;
        current->finish_callback();
        return;
      }
      callback->second();
    }
    for (auto* node : current->next) {
      DrawRecursive(node);
    }
    current->finish_callback();
  }
}
void SceneUIParser::Clear() {
  ClearElement(Element::Count);
}

void SceneUIParser::ClearElement(Element what) {
  for (Element it = Element::Menu; it != Element::Count;
       it = utils::IncEnum(it)) {
    if (what == Element::Count || what == it) {
      if (nodes_[it]) {
        VLOG(1) << "Clearing node " << nodes_[it]->node_name;
        ClearChildren(nodes_[it]);
        delete nodes_[it];
        nodes_[it] = nullptr;
      }
    }
  }
}

SceneUIParser::Element SceneUIParser::ElementFromString(
    const std::string& element_name) {
  if (element_name == std::string(kMenuNodeName)) {
    return Element::Menu;
  } else if (element_name == std::string(kSceneNodeName)) {
    return Element::Scene;
  }
  return Element::Count;
}

void SceneUIParser::ClearChildren(SceneUIParserNode* current) {
  if (!current) {
    return;
  }
  for (auto* node : current->next) {
    ClearChildren(node);
    if (node) {
      delete node;
    }
  }
}

void SceneUIParser::Init() {
  VLOG(3) << __FUNCTION__;
  for (Element it = Element::Menu; it != Element::Count;
       it = utils::IncEnum(it)) {
    nodes_[it] = nullptr;
  }
}

SceneUIParser::~SceneUIParser() {
  Clear();
}
