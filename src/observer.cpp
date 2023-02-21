#include <MEP-3D/observer.hpp>

namespace mep {
int Observer::id_couter_ = 0;

Observer::Observer() : id_(id_couter_++) {}

bool Observer::IsEqual(Observer* obs) const {
  return obs && id_ == obs->id_;
}

bool Observer::GetObserverId() const {
  return id_;
}
}  // namespace mep