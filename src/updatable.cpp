#include <MEP-3D/updatable.hpp>

void Updatable::Updated() {
  changed_ = false;
}

void Updatable::Changed() {
  changed_ = true;
}

bool Updatable::ShouldUpdate() const {
  return changed_;
}

void Updatable::UpdateInt(UpdateCallback update) {
  if (ShouldUpdate()) {
    update();
    Updated();
  }
}
