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
  enum class Element : unsigned int { Menu = 0, Scene = 1, Count };
  SceneUIParser(const std::unordered_map<int, Callback>& handler_map);
  virtual void Draw();
  virtual void Parse(const std::string& json) = 0;
  virtual void Clear();
  Element ElementFromString(const std::string& element_name);
  virtual ~SceneUIParser();

 protected:
  std::unordered_map<int, Callback> handler_map_;
  void DrawRecursive(SceneUIParserNode* current);
  void ClearChildren(SceneUIParserNode* current);
  void ClearElement(Element what);
  void Init();
  std::unordered_map<Element, SceneUIParserNode*> nodes_;
  nlohmann::json root_;
};

#endif
