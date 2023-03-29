#ifndef PARSER_HPP
#define PARSER_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace mep {
class Parser {
 public:
  using CallbackWithReplay = std::function<bool()>;
  using Callback = std::function<void()>;
  struct ParserNode {
    CallbackWithReplay start_callback;
    Callback finish_callback;
    std::string node_name;
    int return_code;
    std::vector<ParserNode*> next;
    ParserNode();
  };
  enum class Element : unsigned int { Menu = 0, Scene = 1, Popup, Count };
  enum class Method : unsigned int {
    Override = 1 << 0,
    FillMissing = 1 << 1,
    DoNotUseDefault = 2 << 1,
    FillAndOverride = Override | FillMissing
  };
  Parser();
  void SetHandler(const std::unordered_map<int, Callback>& handler_map);
  void MergeHandler(const std::unordered_map<int, Callback>& handler_map,
                    Method method = Method::FillAndOverride);
  virtual void Draw();
  virtual void Parse(const std::string& json) = 0;
  virtual void Clear();
  Element ElementFromString(const std::string& element_name);
  virtual ~Parser();

 protected:
  std::unordered_map<int, Callback> handler_map_;
  void DrawRecursive(ParserNode* current);
  void ClearChildren(ParserNode* current);
  void ClearElement(Element what);
  void Init();
  std::vector<ParserNode*> windows_root_;
  std::unordered_map<Element, ParserNode*> nodes_;
  nlohmann::json root_;
};
}  // namespace mep

#endif
