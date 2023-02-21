#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

#include <MEP-3D/common.hpp>
#include <MEP-3D/features.hpp>
#include <glm/glm.hpp>

#define NOTREACHED() assert(false);

namespace mep {
namespace utils {
void Init(int argc, char* argv[]);
void InitLogging(int argc, char* argv[]);
void InitFeatures(int argc, char* argv[]);

glm::vec3 AxisToGlm(Axis axis);
float DegToRadians(float degress);
float RadiansToDeg(float radians);
std::string LoadFromFile(const std::string& file_path);
template <typename Enum>
inline Enum IncEnum(Enum value) {
  return static_cast<Enum>(static_cast<int>(value) + 1);
}
template <typename Enum>
inline bool Contains(Enum lval, Enum rval) {
  return static_cast<unsigned int>(lval) & static_cast<unsigned int>(rval);
}
}  // namespace utils
}  // namespace mep

#endif