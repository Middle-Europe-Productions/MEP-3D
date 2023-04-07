#ifndef IMGUI_ENUM_HPP
#define IMGUI_ENUM_HPP

#include <MEP-3D/utils.hpp>

#include <imgui.h>
#include <vector>

#include <glog/logging.h>

namespace ImGui {
namespace Widget {
template <typename Enum>
class EnumWidget {
 public:
  EnumWidget(int start, int finish)
      : start_(start),
        finish_(finish),
        selected_(0),
        id_name_("##" + mep::utils::RandomString(10)) {
    Init();
  }
  void Draw() {
    DCHECK(selected_ < nodes_.size() &&
           selected_ != (std::numeric_limits<std::size_t>::max)());
    if (ImGui::BeginCombo(id_name_.c_str(), nodes_[selected_].c_str())) {
      for (std::size_t n = 0; n < nodes_.size(); n++)
        if (ImGui::Selectable(nodes_[n].c_str(), n == selected_)) {
          selected_ = n;
        }
      ImGui::EndCombo();
    }
  }
  Enum GetCurrent() const {
    return static_cast<Enum>(selected_ + static_cast<int>(start_));
  }

 private:
  void Init() {
    size_ = static_cast<int>(finish_) - static_cast<int>(start_);
    for (auto start = static_cast<Enum>(start_);
         start != static_cast<Enum>(finish_);
         start = mep::utils::IncEnum(start)) {
      nodes_.push_back(mep::ToString(start));
    }
  }
  int start_;
  int finish_;
  std::size_t selected_;
  std::size_t size_;
  const std::string id_name_;
  std::vector<std::string> nodes_;
};
}  // namespace Widget
}  // namespace ImGui

#endif
