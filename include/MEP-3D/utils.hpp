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
void InitOpenGLogging();

glm::vec3 AxisToGlm(Axis axis);
float DegToRadians(float degress);
float RadiansToDeg(float radians);
std::string LoadFromFile(const std::string& file_path);
bool SaveToFile(const std::string& file_path, const std::string& content);
std::string RandomString(std::size_t size);
template <typename Type>
inline Type Clamp(Type value, Type upper, Type lower) {
  return value > upper ? upper : value < lower ? lower : value;
}
template <typename Type>
inline Type Normalize(Type pos, Type view, Type offset) {
  return (pos - offset) / view;
}
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