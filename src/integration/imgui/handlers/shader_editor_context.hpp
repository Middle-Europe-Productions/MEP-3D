#ifndef SHADER_EDITOR_CONTEXT_HPP
#define SHADER_EDITOR_CONTEXT_HPP

#include <MEP-3D/template/scene_ui_layer_handler_context.hpp>

#include <text-editor/TextEditor.h>

namespace mep {
class ShaderEditorContext : public UI_HANDLER_BASE(SceneUILayer) {
 public:
  void SelectShader(int selected_shader,
                    std::vector<std::unique_ptr<Shader>>& shaders);
  void DrawShader();
  void DrawUtilityMenu();
  void UpdateMarkers();
  void TranslateErrorToMap(TextEditor::ErrorMarkers& error_marker,
                           const std::string& message);
  bool Compile();

 protected:
  int selected_shader_ = -1;
  bool should_draw_vertex = false;
  bool should_draw_fragment = false;

  bool eval_vertex_change = false;
  bool eval_fragment_change = false;
  bool vertex_changed_ = false;
  bool fragment_changed_ = false;
  // TextEditor
  TextEditor editor_[2];
  TextEditor::ErrorMarkers fragment_markers;
  TextEditor::ErrorMarkers vertex_markers;
  // mep::Shader
  Shader* shader_ = nullptr;
  ShaderError last_error;
};
}  // namespace mep

#endif