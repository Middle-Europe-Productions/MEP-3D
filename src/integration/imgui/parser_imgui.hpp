#ifndef PARSER_IMGUI_HPP
#define PARSER_IMGUI_HPP

#include <MEP-3D/parser.hpp>

#include <queue>

namespace mep {
class ImguiPopupController {
 public:
  static ImguiPopupController& Get();
  void RequestPopupOpen(const std::string& name);
  bool OpenAllOnCurrentStack();

 private:
  ImguiPopupController() = default;
  std::queue<std::string> requested_popups_;
};

class ParserImGui : public Parser {
 public:
  ParserImGui();
  virtual void Parse(const std::string& json) override;

 private:
  std::vector<ParserNode*> ParseWindow(nlohmann::json& json_data);
  ParserNode* ParseMenuItem(nlohmann::json& json_data);
  ParserNode* ParseSceneItem(nlohmann::json& json_data, int depth);
  ParserNode* ParsePopupItem(nlohmann::json& json_data, int depth);
};
}  // namespace mep

#endif