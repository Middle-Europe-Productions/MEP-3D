#ifndef SCENE_UI_PARSER_HPP
#define SCENE_UI_PARSER_HPP

#include <nlohmann/json.hpp>
#include <string>

class SceneUIParser {
 public:
  using CallbackWithReplay = std::function<bool()>;
  using Callback = std::function<void()>;
  struct SceneUIParserNode {
    CallbackWithReplay start_callback;
    Callback finish_callback;
    std::string node_name;
    int return_code;
    std::vector<SceneUIParserNode*> next;
    SceneUIParserNode();
  };
  SceneUIParser();
  int DrawWithReplay();
  virtual void Parse(const std::string& json) = 0;
  virtual void Clear();
  virtual ~SceneUIParser();

 protected:
  int DrawRecursiveWithResult(SceneUIParserNode* current);
  void ClearChildren(SceneUIParserNode* current);
  SceneUIParserNode* head_;
  nlohmann::json root_;
};

#endif
