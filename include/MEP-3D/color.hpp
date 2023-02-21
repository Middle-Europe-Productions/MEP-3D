#ifndef COLOR_HPP
#define COLOR_HPP

#include <MEP-3D/common.hpp>

namespace mep {
class Color {
 public:
  constexpr Color() : r_(0), g_(0), b_(0), a_(0) {}
  constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
      : r_(r), g_(g), b_(b), a_(a) {}

  constexpr Uint8 R() const { return r_; }
  constexpr Uint8 G() const { return g_; }
  constexpr Uint8 B() const { return b_; }
  constexpr Uint8 A() const { return a_; }
  constexpr float Rf() const { return static_cast<float>(r_) / 255.0f; }
  constexpr float Gf() const { return static_cast<float>(g_) / 255.0f; }
  constexpr float Bf() const { return static_cast<float>(b_) / 255.0f; }
  constexpr float Af() const { return static_cast<float>(a_) / 255.0f; }

 private:
  Uint8 r_;
  Uint8 g_;
  Uint8 b_;
  Uint8 a_;
};

constexpr Color Black = Color(0, 0, 0, 255);
constexpr Color White = Color(255, 255, 255, 255);
constexpr Color Blue = Color(0, 255, 0, 255);
constexpr Color Green = Color(0, 0, 255, 255);
constexpr Color Red = Color(255, 0, 0, 255);
};  // namespace mep

#endif