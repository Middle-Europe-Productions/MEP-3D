#include <MEP-3D/drawable.hpp>

namespace mep {
Drawable::Drawable() : should_draw_(true) {}

bool Drawable::ShouldDraw() const {
  return should_draw_;
}

void Drawable::SetShouldDraw(bool should_draw) {
  should_draw_ = should_draw;
}

bool& Drawable::GetShouldDraw() {
  return should_draw_;
}
}  // namespace mep
