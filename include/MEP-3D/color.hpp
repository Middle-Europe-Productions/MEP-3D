#ifndef COLOR_HPP
#define COLOR_HPP

#include <MEP-3D/common.hpp>
#include <iostream>

namespace mep {
struct Color {
 public:
  constexpr Color() : r(0), g(0), b(0), a(0) {}
  constexpr Color(Uint8 r_in, Uint8 g_in, Uint8 b_in, Uint8 a_in = 255)
      : r(r_in), g(g_in), b(b_in), a(a_in) {}

  constexpr Uint8 R() const { return r; }
  constexpr Uint8 G() const { return g; }
  constexpr Uint8 B() const { return b; }
  constexpr Uint8 A() const { return a; }
  constexpr float Rf() const { return static_cast<float>(r) / 255.0f; }
  constexpr float Gf() const { return static_cast<float>(g) / 255.0f; }
  constexpr float Bf() const { return static_cast<float>(b) / 255.0f; }
  constexpr float Af() const { return static_cast<float>(a) / 255.0f; }
  constexpr Uint32 ARGB() const {
    return (static_cast<Uint32>(a) << 24) | (static_cast<Uint32>(r) << 16) |
           (static_cast<Uint32>(g) << 8) | static_cast<Uint32>(b);
  };
  constexpr Uint32 ABGR() const {
    return (static_cast<Uint32>(a) << 24) | (static_cast<Uint32>(b) << 16) |
           (static_cast<Uint32>(g) << 8) | static_cast<Uint32>(r);
  };
  constexpr operator Uint32() { return ARGB(); }
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
};

constexpr Color Black = Color(0, 0, 0, 255);
constexpr Color White = Color(255, 255, 255, 255);
constexpr Color Blue = Color(0, 255, 0, 255);
constexpr Color Green = Color(0, 0, 255, 255);
constexpr Color Red = Color(255, 0, 0, 255);

inline std::ostream& operator<<(std::ostream& out, const Color& color) {
  out << "R: " << static_cast<int>(color.r)
      << ", G: " << static_cast<int>(color.g)
      << ", B: " << static_cast<int>(color.b)
      << ", A: " << static_cast<int>(color.a);
  return out;
}

};  // namespace mep

#endif