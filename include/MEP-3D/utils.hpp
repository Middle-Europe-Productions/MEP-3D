#ifndef UTILS_HPP
#define UTILS_HPP

#include <MEP-3D/common.hpp>
#include <glm/glm.hpp>

namespace utils {
glm::vec3 AxisToGlm(Axis axis);
float DegToRadians(float degress);
float RadiansToDeg(float radians);
template <typename Enum>
inline Enum IncEnum(Enum value) {
  return static_cast<Enum>(static_cast<int>(value) + 1);
}
}  // namespace utils

#endif