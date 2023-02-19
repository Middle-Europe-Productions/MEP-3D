#ifndef UI_HANDLERS_IMPL
#define UI_HANDLERS_IMPL

#include <MEP-3D/non_copyable.hpp>
#include <MEP-3D/template/scene_ui_layer.hpp>

#include <functional>
#include <vector>

template <typename Context, typename Handler>
inline int MakeHandlerInfo(int element) {
  static_assert(std::is_default_constructible<Handler>::value);
  Context::Get().AddHandler(new Handler());
  return element;
};

#define UI_CONTEXT_NAME(context_class) scene_ui_layer_context_##context_class
#define UI_HANDLER_BASE(context_class) scene_ui_handler_base_##context_class

#define UI_CONTEXT(context_class)                                            \
  class UI_HANDLER_BASE(context_class) {                                     \
   public:                                                                   \
    UI_HANDLER_BASE(context_class)() = default;                              \
    virtual void Draw() = 0;                                                 \
    virtual int GetId() const = 0;                                           \
    virtual ~UI_HANDLER_BASE(context_class)() = default;                     \
    void RegisterContext(context_class* context) {                           \
      DCHECK(context);                                                       \
      context_ = context;                                                    \
    }                                                                        \
    context_class* GetContext() { return context_; }                         \
    const context_class* GetContext() const { return context_; }             \
                                                                             \
   private:                                                                  \
    context_class* context_;                                                 \
  };                                                                         \
  class UI_CONTEXT_NAME(context_class) : public NonCopyable {                \
   public:                                                                   \
    static UI_CONTEXT_NAME(context_class) & Get();                           \
    void AddHandler(scene_ui_handler_base_SceneUILayer* handler) {           \
      handlers_.push_back(handler);                                          \
    }                                                                        \
    void Clear() {                                                           \
      for (auto* ele : handlers_) {                                          \
        delete ele;                                                          \
        ele = nullptr;                                                       \
      }                                                                      \
      handlers_.clear();                                                     \
    }                                                                        \
    std::unordered_map<int, std::function<void()>> GetHandlerMapWithContext( \
        context_class* context) {                                            \
      std::unordered_map<int, std::function<void()>> handler_map;            \
      for (auto* ele : handlers_) {                                          \
        ele->RegisterContext(context);                                       \
        handler_map[ele->GetId()] =                                          \
            std::bind(&scene_ui_handler_base_SceneUILayer::Draw, ele);       \
      }                                                                      \
      return handler_map;                                                    \
    }                                                                        \
                                                                             \
   private:                                                                  \
    std::vector<scene_ui_handler_base_SceneUILayer*> handlers_;              \
    UI_CONTEXT_NAME(context_class)() = default;                              \
    static bool link_;                                                       \
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
      : public UI_HANDLER_BASE(context_class) {                                \
   public:                                                                     \
    SCENE_UI_HANDLER_NAME(element_id, context_class)() = default;              \
    ~SCENE_UI_HANDLER_NAME(element_id, context_class)() = default;             \
    void Draw() override;                                                      \
    int GetId() const override { return element_id_; }                         \
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
  UI_HANDLER_D(element_class, element_id, context_class, int)

#endif
