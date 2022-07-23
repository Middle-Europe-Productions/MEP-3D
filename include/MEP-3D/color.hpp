#ifndef COLOR_HPP
#define COLOR_HPP

#include <MEP-3D/common.hpp>

class Color {
 public:
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

#endif