#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <MEP-3D/color.hpp>
#include <MEP-3D/vector.hpp>

#include <vector>

namespace mep {
struct CanvasConfig {
  Color overlay_color = White;
  Color line_color = White;
  Color point_color = White;
  float point_radius = 10.0f;
  /* Point provided in correct order */
  std::vector<Vec2f> control_points = {{0.0f, 0.0f},
                                       {0.5f, 1.0f},
                                       {1.0f, 0.0f}};
};

class PointCanvas {
 public:
  PointCanvas(const CanvasConfig& config = CanvasConfig());
  bool DrawAndNotify();
  void Reset();
  const std::vector<mep::Vec2f>& GetPoints() const;

 private:
  bool EvaluateItemDragged(const Vec2f& view, const Vec2f& offset);
  void SortPoints();

  std::vector<Vec2f> control_points_;
  std::vector<Vec2f>::iterator selected_point_;
  CanvasConfig config_;
};
}  // namespace mep

#endif
