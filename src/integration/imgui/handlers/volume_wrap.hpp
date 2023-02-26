#ifndef VOLUME_WRAP_HPP
#define VOLUME_WRAP_HPP

#include <MEP-3D/texture_3d.hpp>
#include <MEP-3D/vector.hpp>
#include <MEP-3D/volume.hpp>

#include "../enum_widget.hpp"
#include "resource_wrap.hpp"

namespace mep {
struct VolumeWrap : public ResourceWrap {
  Vec3i size = {0, 0, 0};
  ImGui::Widget::EnumWidget<Texture3D::Type> enum_widget;

  VolumeWrap();
  template <typename Context>
  std::unique_ptr<Volume> Create(Context* context);
  bool FindSize(const std::string& text);
  bool SetFilePath(const std::filesystem::path& path) override;
};
}  // namespace mep

#endif
