#include <glog/logging.h>
#include <MEP-3D/common_names.hpp>
#include <MEP-3D/parser.hpp>
#include <MEP-3D/utils.hpp>

namespace mep {
namespace {
constexpr auto DoNothing = []() {};
constexpr auto DoNothingWithReplay = []() -> bool { return true; };
};  // namespace

Parser::ParserNode::ParserNode()
    : start_callback(DoNothingWithReplay),
      finish_callback(DoNothing),
      node_name(""),
      return_code(-1) {}

Parser::Parser() : handler_map_() {
  Init();
}

void Parser::SetHandler(ParserHandlerMap handler_map) {
  DCHECK(handler_map.size() != 0);
  handler_map_ = std::move(handler_map);
}

void Parser::MergeHandler(ParserHandlerMap handler_map, Method method) {
  for (auto& node : handler_map) {
    auto it = handler_map_.find(node.first);
    if (it != handler_map_.end()) {
      if (utils::Contains(method, Method::Override)) {
        it->second = std::move(node.second);
      }
    } else {
      if (utils::Contains(method, Method::FillMissing)) {
        handler_map_[node.first] = std::move(node.second);
      }
    }
  }
}

void Parser::Draw() {
  for (auto* node : windows_root_) {
    DrawRecursive(node);
  }
}

void Parser::DrawRecursive(ParserNode* current) {
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
      callback->second->Draw();
    }
    for (auto* node : current->next) {
      DrawRecursive(node);
    }
    current->finish_callback();
  }
}
void Parser::Clear() {
  ClearElement(Element::Count);
  for (auto& node : handler_map_) {
    DCHECK(node.second);
    node.second->TearDown();
  }
}

void Parser::SetUpHandlers() {
  VLOG(3) << __func__ << ", number of handlers: " << handler_map_.size();
  for (auto& node : handler_map_) {
    DCHECK(node.second);
    node.second->SetUp();
  }
}

void Parser::TearDownHandlers() {
  VLOG(3) << __func__ << ", number of handlers: " << handler_map_.size();
  for (auto& node : handler_map_) {
    DCHECK(node.second);
    node.second->TearDown();
  }
}

void Parser::ClearElement(Element what) {
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

Parser::Element Parser::ElementFromString(const std::string& element_name) {
  if (element_name == std::string(kMenuNodeName)) {
    return Element::Menu;
  } else if (element_name == std::string(kSceneNodeName)) {
    return Element::Scene;
  } else if (element_name == std::string(kPopupNodeName)) {
    return Element::Popup;
  }
  return Element::Count;
}

void Parser::ClearChildren(ParserNode* current) {
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

void Parser::Init() {
  VLOG(3) << __FUNCTION__;
  for (Element it = Element::Menu; it != Element::Count;
       it = utils::IncEnum(it)) {
    nodes_[it] = nullptr;
  }
}

Parser::~Parser() {
  Clear();
}
}  // namespace mep
