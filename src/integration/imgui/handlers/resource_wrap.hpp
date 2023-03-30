#ifndef RESOURCE_WRAP_HPP
#define RESOURCE_WRAP_HPP

#include <MEP-3D/platform/platform_delegate.hpp>
#include <MEP-3D/template/scene_ui_layer_handler_context.hpp>
#include <filesystem>

namespace {
constexpr int kMaxPathSize = 256;
};

namespace mep {
struct ResourceWrap : public UI_HANDLER_BASE(SceneUILayer) {
  enum Active : int { Shader = 1 << 0, Material = 1 << 1, Texture = 1 << 2 };
  char display_name[kMaxPathSize];
  std::filesystem::path buffer_path;
  int selected_shader_ = -1;
  int selected_material_ = -1;
  int selected_texture_ = -1;
  bool init_ = false;
  int active_ = 0;
  ResourceWrap();
  ResourceWrap(int active);
  bool IsInit() const;
  void Uninitialize();
  virtual bool SetFilePath(const std::filesystem::path& path);
  bool ValidData() const;
  template <typename Context>
  void Init(Context* context, const FileFilter& filter) noexcept;

  template <typename Resource>
  void DrawSelectable(Resource* resource);

  template <typename Resource, typename Context>
  void EvalSelectable(Resource* resource, Context* context);
};

template <typename Resource, typename Context>
void ResourceWrap::EvalSelectable(Resource* resource, Context* context) {
  DCHECK(resource);
  DCHECK(context);
  if (selected_shader_ != -1) {
    resource->Bind(
        context->GetScenePtr()->GetShaders()[selected_shader_].get());
  }
  if (selected_texture_ != -1) {
    resource->Bind(
        context->GetScenePtr()->GetTexture()[selected_texture_].get());
  }
  if (selected_material_ != -1) {
    resource->Bind(
        context->GetScenePtr()->GetMaterial()[selected_material_].get());
  }
}

}  // namespace mep

#endif
