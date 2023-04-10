#include "point_canvas.hpp"

#include <MEP-3D/utils.hpp>

#include <glog/logging.h>
#include <imgui.h>
#include <algorithm>

namespace mep {
namespace {
constexpr char kInvButtonId[] = "inv_button_id";
ImVec2 ToImVec2(Vec2f vec) {
  return ImVec2{vec.x_, vec.y_};
}

Vec2f GetActualPosition(Vec2f pos, Vec2f view, Vec2f offset) {
  return (pos * view) + offset;
}

bool CompareFunction(const Point& x, const Point& y) {
  return x.position.x_ < y.position.x_;
}

};  // namespace
PointCanvas::PointCanvas(const CanvasConfig& config) : config_(config) {
  Reset();
}

bool PointCanvas::DrawAndNotify() {
  const ImGuiIO& io = ImGui::GetIO();
  ImVec2 canvas_size = ImGui::GetContentRegionAvail();

  ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  canvas_size[1] = config_.canvas_size_y;
  draw_list->PushClipRect(
      canvas_pos,
      ImVec2(canvas_pos[0] + canvas_size[0], canvas_pos[1] + canvas_size[1]),
      true);

  const Vec2f view_scale(canvas_size.x, -canvas_size.y);
  const Vec2f view_offset(canvas_pos.x, canvas_pos.y + canvas_size.y);

  draw_list->AddRect(
      canvas_pos,
      ImVec2(canvas_pos[0] + canvas_size[0], canvas_pos[1] + canvas_size[1]),
      config_.overlay_color.ABGR());

  ImGui::InvisibleButton(kInvButtonId, canvas_size);

  bool points_changed = EvaluateItemDragged(view_scale, view_offset);

  std::vector<ImVec2> transformed_points_position_;
  for (const auto& pt : control_points_) {
    ImVec2 actual_position =
        ToImVec2(GetActualPosition(pt.position, view_scale, view_offset));
    transformed_points_position_.push_back(actual_position);
  }
  draw_list->AddPolyline(transformed_points_position_.data(),
                         (int)transformed_points_position_.size(),
                         config_.line_color, false, 2.f);
  std::size_t pos = 0;
  for (const auto& pt : control_points_) {
    draw_list->AddCircleFilled(transformed_points_position_[pos++],
                               config_.point_radius, pt.color.ABGR());
  }
  draw_list->PopClipRect();
  return points_changed;
}

void PointCanvas::Reset() {
  control_points_ = config_.control_points;
  selected_point_ = control_points_.end();
}

const std::vector<Point>& PointCanvas::GetPoints() const {
  return control_points_;
}

bool PointCanvas::EvaluateItemDragged(const Vec2f& view, const Vec2f& offset) {
  const ImGuiIO& io = ImGui::GetIO();
  ImVec2 canvas_min = ImGui::GetItemRectMin();
  ImVec2 canvas_max = ImGui::GetItemRectMax();
  bool left_pressed = io.MouseDown[0];
  bool right_pressed = io.MouseDown[1];
  bool left_released = io.MouseReleased[0];
  bool right_released = io.MouseReleased[1];
  Vec2f clipped_position =
      Vec2f((std::min)((std::max)(io.MousePos.x, canvas_min.x), canvas_max.x),
            (std::min)((std::max)(io.MousePos.y, canvas_min.y), canvas_max.y));

  Vec2f normalized_mouse_pos = utils::Normalize(clipped_position, view, offset);
  const auto element_finder = [&view, &offset, &clipped_position,
                               radius =
                                   config_.point_radius](const Point& point) {
    Vec2f actual_position = GetActualPosition(point.position, view, offset);
    float distance = (actual_position - clipped_position).Length();
    return radius >= distance;
  };
  if (ImGui::IsItemHovered() && left_pressed || right_pressed) {
    if (left_pressed) {
      if (selected_point_ != control_points_.end()) {
        if ((selected_point_ == control_points_.begin() ||
             selected_point_ == control_points_.end() - 1) &&
            selected_point_->position.y_ == normalized_mouse_pos.y_) {
          return false;
        }
        if (selected_point_->position == normalized_mouse_pos) {
          return false;
        }
        if (!selected_point_->status) {
          selected_point_->status = true;
          selected_point_->color = config_.point_color;
        }
        selected_point_->position = normalized_mouse_pos;

        if (selected_point_ + 1 != control_points_.end() &&
            selected_point_->position.x_ >=
                (selected_point_ + 1)->position.x_) {
          selected_point_->position.x_ = (selected_point_ + 1)->position.x_;
        }
        if (selected_point_ != control_points_.begin() &&
            selected_point_->position.x_ <=
                (selected_point_ - 1)->position.x_) {
          selected_point_->position.x_ = (selected_point_ - 1)->position.x_;
        }

        if (selected_point_ == control_points_.begin()) {
          selected_point_->position.x_ = 0.0;
        } else if (selected_point_ == control_points_.end() - 1) {
          selected_point_->position.x_ = 1.0;
        }
        SortPoints();
        return true;
      } else {
        selected_point_ = std::find_if(control_points_.begin(),
                                       control_points_.end(), element_finder);
      }
    }
  }
  if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))) {
    selected_point_ = std::find_if(control_points_.begin(),
                                   control_points_.end(), element_finder);
    if (selected_point_ == control_points_.begin() ||
        selected_point_ == control_points_.end() ||
        selected_point_ == control_points_.end() - 1) {
      return false;
    }
    control_points_.erase(selected_point_);
    selected_point_ = control_points_.end();
    SortPoints();
  }
  if (right_released) {
    selected_point_ = std::find_if(control_points_.begin(),
                                   control_points_.end(), element_finder);
    // we add the element on right click
    if (selected_point_ == control_points_.end()) {
      control_points_.push_back(
          {normalized_mouse_pos, true, config_.point_color});
      selected_point_ = control_points_.end();
      SortPoints();
      return true;
    }
    if (selected_point_ == control_points_.begin() ||
        selected_point_ == control_points_.end() - 1) {
      return false;
    }
    if (!selected_point_->status) {
      selected_point_->status = true;
      selected_point_->color = config_.point_color;
    } else {
      selected_point_->status = false;
      selected_point_->color = config_.point_color_on_disabled;
    }
    return true;
  } else if (left_released) {
    selected_point_ = control_points_.end();
  }
  return false;
}

void PointCanvas::SortPoints() {
  std::sort(control_points_.begin(), control_points_.end(), CompareFunction);
}

};  // namespace mep
