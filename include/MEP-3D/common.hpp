#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>

using Uint8 = unsigned char;

namespace mep {
extern const char* kDefaultTexturePath;

enum class Axis { X, Y, Z };

enum class CommonUniform : unsigned int {
  Projection = 10000,
  Model = 10001,
  View,
  Position
};

std::string ToString(const Axis& axis);
std::ostream& operator<<(std::ostream& out, const Axis& axis);
}  // namespace mep

#endif