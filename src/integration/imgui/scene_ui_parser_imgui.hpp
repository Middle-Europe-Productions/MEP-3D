#ifndef SCENE_UI_PARSER_IMGUI_HPP
#define SCENE_UI_PARSER_IMGUI_HPP

#include <MEP-3D/scene_ui_parser.hpp>

class SceneUIParserImGui : public SceneUIParser {
 public:
  SceneUIParserImGui();
  virtual void Parse(const std::string& json) override;

 private:
  SceneUIParserNode* ParseMenuItem(nlohmann::json& json_data);
  SceneUIParserNode* ParseSceneItem(nlohmann::json& json_data, int depth);
};

#endif