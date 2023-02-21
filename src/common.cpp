#include <MEP-3D/common.hpp>

namespace mep {
const char* kDefaultTexturePath = "textures/";

std::string ToString(const Axis& axis) {
  switch (axis) {
    case Axis::X:
      return "X";
    case Axis::Y:
      return "Y";
    case Axis::Z:
      return "Z";
  }
  return "";
}

std::ostream& operator<<(std::ostream& out, const Axis& axis) {
  out << ToString(axis);
  return out;
}
}  // namespace mep
