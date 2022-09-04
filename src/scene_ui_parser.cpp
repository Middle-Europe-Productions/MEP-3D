#include <glog/logging.h>
#include <MEP-3D/scene_ui_parser.hpp>

namespace {
constexpr auto DoNothing = []() {};
constexpr auto DoNothingWithReplay = []() -> bool { return false; };
};  // namespace

SceneUIParser::SceneUIParserNode::SceneUIParserNode()
    : start_callback(DoNothingWithReplay),
      finish_callback(DoNothing),
      node_name(""),
      return_code(-1) {}

SceneUIParser::SceneUIParser() : head_(nullptr) {}

int SceneUIParser::DrawWithReplay() {
  int response = DrawRecursiveWithResult(head_);
  if (response != -1)
    LOG(INFO) << "Draw with replay " << response;
  return response;
}

int SceneUIParser::DrawRecursiveWithResult(SceneUIParserNode* current) {
  if (!current) {
    return -1;
  }
  if (current->start_callback()) {
    if (current->next.size() == 0) {
      current->finish_callback();
      return current->return_code;
    }
    int return_code = -1;
    for (auto* node : current->next) {
      int local = DrawRecursiveWithResult(node);
      if (local > return_code) {
        return_code = local;
      }
    }
    current->finish_callback();
    return return_code;
  }
  return -1;
}
void SceneUIParser::Clear() {
  if (head_) {
    ClearChildren(head_);
    delete head_;
    head_ = nullptr;
  }
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

SceneUIParser::~SceneUIParser() {
  Clear();
}
