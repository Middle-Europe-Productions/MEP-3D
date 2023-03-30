#ifndef UI_HANDLERS_IMPL
#define UI_HANDLERS_IMPL

#include <MEP-3D/non_copyable.hpp>
#include <MEP-3D/parser_handler.hpp>
#include <MEP-3D/template/scene_ui_layer.hpp>

#include <functional>
#include <vector>

template <typename Context, typename Handler>
inline int MakeHandlerInfo(int element) {
  static_assert(std::is_default_constructible<Handler>::value);
  Context::Get().AddHandler(element, std::make_unique<Handler>());
  return element;
};

#define UI_CONTEXT_NAME(context_class) scene_ui_layer_context_##context_class
#define UI_HANDLER_BASE(context_class) scene_ui_handler_base_##context_class

#define UI_CONTEXT(context_class)                                              \
  class UI_HANDLER_BASE(context_class) : public ParserHandler {                \
   public:                                                                     \
    void SetUp() override {}                                                   \
    void TearDown() override {}                                                \
    virtual void Draw() {}                                                     \
    mep::Scene* Scene() { return scene_; }                                     \
    const mep::Scene* Scene() const { return scene_; }                         \
    void RegisterContext(mep::Scene* context) {                                \
      DCHECK(context);                                                         \
      scene_ = context;                                                        \
    }                                                                          \
    context_class* GetContext() { return context_; }                           \
    void RegisterContext(context_class* context) {                             \
      DCHECK(context);                                                         \
      context_ = context;                                                      \
    }                                                                          \
                                                                               \
   public:                                                                     \
    mep::Scene* scene_ = nullptr;                                              \
    context_class* context_ = nullptr;                                         \
  };                                                                           \
  class UI_CONTEXT_NAME(context_class) : public NonCopyable {                  \
   public:                                                                     \
    static UI_CONTEXT_NAME(context_class) & Get();                             \
    void AddHandler(int id,                                                    \
                    std::unique_ptr<UI_HANDLER_BASE(context_class)> handler) { \
      handlers_[id] = std::move(handler);                                      \
    }                                                                          \
                                                                               \
    std::unordered_map<int, std::unique_ptr<ParserHandler>>                    \
    GetHandlerMapWithContext(context_class* context) {                         \
      std::unordered_map<int, std::unique_ptr<ParserHandler>> parser_map;      \
      for (auto& node : handlers_) {                                           \
        node.second->RegisterContext(context);                                 \
        parser_map[node.first] = std::move(node.second);                       \
      }                                                                        \
      handlers_.clear();                                                       \
      return std::move(parser_map);                                            \
    }                                                                          \
                                                                               \
   private:                                                                    \
    std::unordered_map<int, std::unique_ptr<UI_HANDLER_BASE(context_class)>>   \
        handlers_;                                                             \
    UI_CONTEXT_NAME(context_class)() = default;                                \
  };

#define LINK_UI_CONTEXT_AND_HANDLERS(context_class)                        \
  UI_CONTEXT_NAME(context_class) & UI_CONTEXT_NAME(context_class)::Get() { \
    static UI_CONTEXT_NAME(context_class) instance;                        \
    return instance;                                                       \
  }

#define GET_UI_CONTEXT(context_class) UI_CONTEXT_NAME(context_class)::Get()

#define SCENE_UI_HANDLER_NAME(element_id, context_class) \
  scene_ui_layer_handler_##element_id##_##context_class

#define UI_HANDLER_D(element_class, element_id, context_class, data_structure) \
  class SCENE_UI_HANDLER_NAME(element_id, context_class)                       \
      : public data_structure {                                                \
   public:                                                                     \
    SCENE_UI_HANDLER_NAME(element_id, context_class)() = default;              \
    ~SCENE_UI_HANDLER_NAME(element_id, context_class)() = default;             \
    void Draw() override;                                                      \
    data_structure& GetData() { return data_; }                                \
                                                                               \
   private:                                                                    \
    static int element_id_;                                                    \
    data_structure data_;                                                      \
  };                                                                           \
                                                                               \
  int SCENE_UI_HANDLER_NAME(element_id, context_class)::element_id_ =          \
      MakeHandlerInfo<UI_CONTEXT_NAME(context_class),                          \
                      SCENE_UI_HANDLER_NAME(element_id, context_class)>(       \
          static_cast<int>(element_class::element_id));                        \
                                                                               \
  void SCENE_UI_HANDLER_NAME(element_id, context_class)::Draw()

#define UI_HANDLER(element_class, element_id, context_class) \
  UI_HANDLER_D(element_class, element_id, context_class,     \
               UI_HANDLER_BASE(context_class))

#endif
