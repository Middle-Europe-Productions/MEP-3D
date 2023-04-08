#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <MEP-3D/color.hpp>
#include <MEP-3D/vector.hpp>

#include <vector>

namespace mep {
struct Point {
  Vec2f position;
  bool status;
  Color color;
};

struct CanvasConfig {
  Color overlay_color = White;
  Color line_color = White;
  Color point_color = White;
  Color point_color_on_disabled = DarkGrey;
  float point_radius = 10.0f;
  float canvas_size_y = 150.f;
  /* Point provided in correct order */
  std::vector<Point> control_points = {{{0.0f, 0.0f}, true, White},
                                       {{0.5f, 1.0f}, true, White},
                                       {{1.0f, 0.0f}, true, White}};
};

class PointCanvas {
 public:
  PointCanvas(const CanvasConfig& config = CanvasConfig());
  bool DrawAndNotify();
  void Reset();
  const std::vector<Point>& GetPoints() const;

 private:
  bool EvaluateItemDragged(const Vec2f& view, const Vec2f& offset);
  void SortPoints();

  std::vector<Point> control_points_;
  std::vector<Point>::iterator selected_point_;
  CanvasConfig config_;
};
}  // namespace mep

#endif
