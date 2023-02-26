
#include "volume_wrap.hpp"

#include <MEP-3D/template/scene_ui_layer.hpp>
#include <MEP-3D/texture_3d.hpp>
#include <MEP-3D/vector.hpp>
#include <MEP-3D/volume.hpp>

#include <regex>

namespace {
constexpr auto kVolumeSizeRegex = "(\\d+)x(\\d+)x(\\d+)";
}

namespace mep {
VolumeWrap::VolumeWrap()
    : ResourceWrap((int)Active::Shader),
      enum_widget(static_cast<int>(Texture3D::Type::BYTE_8),
                  static_cast<int>(Texture3D::Type::BYTE_16) + 1) {}

template <typename Context>
std::unique_ptr<Volume> VolumeWrap::Create(Context* context) {
  std::unique_ptr<Volume> volume = std::make_unique<Volume>(display_name);
  volume->LoadFromFile(buffer_path, size, enum_widget.GetCurrent());
  if (selected_shader_ != -1) {
    volume->Bind(context->GetScenePtr()->GetShaders()[selected_shader_].get());
  }
  return volume;
}

bool VolumeWrap::FindSize(const std::string& text) {
  std::regex text_regex(kVolumeSizeRegex);
  auto words_begin = std::sregex_iterator(text.begin(), text.end(), text_regex);
  auto words_end = std::sregex_iterator();
  int counter = 0;
  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    if (counter >= 3) {
      VLOG(3) << "Found more than one match, assuming first.";
      return false;
    }
    try {
      std::smatch match = *i;
      // 3 groups, 1 element
      LOG_IF(WARNING, match.size() != 4) << "Found invalid number of elements";
      bool is_group = true;
      for (auto x : match) {
        if (is_group) {
          is_group = false;
          continue;
        }
        VLOG(6) << "Size match found " << x.str();
        reinterpret_cast<unsigned int*>(&size.x_)[counter++] =
            std::stoi(x.str());
      }
    } catch (std::invalid_argument error) {
      VLOG(3) << "Invalid regex " << error.what();
      return false;
    }
  }
  return counter == 3;
}

bool VolumeWrap::SetFilePath(const std::filesystem::path& path) {
  if (!FindSize(path.filename().string())) {
    VLOG(3) << "Cound not match regex " << kVolumeSizeRegex << " in "
            << path.filename();
  }
  return ResourceWrap::SetFilePath(path);
}

template std::unique_ptr<Volume> VolumeWrap::Create<SceneUILayer>(
    mep::SceneUILayer*);

}  // namespace mep
