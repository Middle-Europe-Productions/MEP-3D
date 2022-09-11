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
template <typename Enum>
inline bool Contains(Enum lval, Enum rval) {
  return static_cast<unsigned int>(lval) & static_cast<unsigned int>(rval);
}
}  // namespace utils

#endif