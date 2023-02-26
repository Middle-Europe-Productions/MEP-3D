#include "resource_wrap.hpp"

#include <MEP-3D/engine.hpp>
#include <MEP-3D/template/scene_ui_layer.hpp>
#include <MEP-3D/template/util_common_draw.hpp>

#include <imgui.h>

namespace mep {

ResourceWrap::ResourceWrap()
    : active_((int)Active::Material | (int)Active::Shader |
              (int)Active::Texture) {}
ResourceWrap::ResourceWrap(int active) : active_(active) {}

bool ResourceWrap::IsInit() const {
  return init_;
}

void ResourceWrap::Uninitialize() {
  DCHECK(init_);
  init_ = false;
}

bool ResourceWrap::SetFilePath(const std::filesystem::path& path) {
  if (path.empty()) {
    return false;
  }
  LOG(INFO) << __func__ << " file path " << path;
  DCHECK(std::filesystem::exists(path));
  buffer_path = path;
  std::string file_string = path.filename().string();
  for (std::size_t i = 0; i < file_string.size(); ++i) {
    if (i >= kMaxPathSize) {
      LOG(WARNING) << "File exceed mac file size";
      return false;
    }
    if (file_string[i] == '.') {
      break;
    }
    display_name[i] = file_string[i];
  }
  return true;
}

bool ResourceWrap::ValidData() const {
  return std::filesystem::exists(buffer_path);
}

template <typename Context>
void ResourceWrap::Init(Context* context, const FileFilter& filter) noexcept {
  if (IsInit()) {
    return;
  }
  DCHECK(context);
  SetFilePath(PlatformDelegate::Get()->OpenFile(
      context->GetEngine()->GetWindow().get(), filter));
  selected_shader_ = context->GetScenePtr()->GetShaders().empty() ? -1 : 0;
  selected_material_ = context->GetScenePtr()->GetMaterial().empty() ? -1 : 0;
  selected_texture_ = context->GetScenePtr()->GetTexture().empty() ? -1 : 0;
  init_ = true;
}

template <typename Resource>
void ResourceWrap::DrawSelectable(Resource* resource) {
  if (active_ & Active::Shader) {
    ImGui::Text("Shader");
    selected_shader_ = UI::Drawer::DrawShaderComboMenu(
        resource->GetScenePtr()->GetShaders(), selected_shader_);
  }
  if (active_ & Active::Texture) {
    ImGui::Text("Texture");
    selected_texture_ = UI::Drawer::DrawTextureComboMenu(
        resource->GetScenePtr()->GetTexture(), selected_texture_);
  }
  if (active_ & Active::Material) {
    ImGui::Text("Material");
    selected_material_ = UI::Drawer::DrawMaterialComboMenu(
        resource->GetScenePtr()->GetMaterial(), selected_material_);
  }
}

template void ResourceWrap::Init<SceneUILayer>(SceneUILayer*,
                                               const FileFilter&);

template void ResourceWrap::DrawSelectable<SceneUILayer>(mep::SceneUILayer*);
}  // namespace mep