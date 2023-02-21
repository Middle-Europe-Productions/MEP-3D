#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <glog/logging.h>

#include <MEP-3D/render_target.hpp>

namespace mep {
class Drawable {
 public:
  Drawable();
  virtual void Draw(RenderTarget& render_target) = 0;
  virtual bool ShouldDraw() const;
  virtual void SetShouldDraw(bool should_draw);
  virtual bool& GetShouldDraw();
  virtual ~Drawable() = default;

 private:
  bool should_draw_;
};
}  // namespace mep

#endif