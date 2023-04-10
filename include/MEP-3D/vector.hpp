#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <string>

namespace mep {
template <typename T>
struct Vec2 {
  constexpr Vec2();
  constexpr Vec2(const T& x, const T& y);
  template <typename U>
  constexpr Vec2(const Vec2<U>& u);
  constexpr Vec2<T>& operator-(const Vec2& x);
  constexpr Vec2<T>& operator+(const Vec2& x);
  constexpr Vec2<T>& operator*(const Vec2& x);
  constexpr Vec2<T>& operator/(const Vec2& x);
  constexpr bool operator==(const Vec2& x) const;
  constexpr T Length() const;
  std::string ToString() const;
  T x_;
  T y_;
};
using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec2u = Vec2<unsigned int>;

template <typename T>
struct Vec3 {
  constexpr Vec3();
  constexpr Vec3(const T& x, const T& y, const T& z);
  template <typename U>
  constexpr Vec3(const Vec3<U>& u);
  constexpr Vec3<T>& operator-(const Vec3& x);
  constexpr Vec3<T>& operator+(const Vec3& x);
  constexpr Vec3<T>& operator*(const Vec3& x);
  constexpr Vec3<T>& operator/(const Vec3& x);
  constexpr bool operator==(const Vec3& x) const;
  constexpr T Length() const;
  constexpr T Max() const;
  constexpr T Min() const;
  std::string ToString() const;
  T x_;
  T y_;
  T z_;
};

using Vec3i = Vec3<int>;
using Vec3f = Vec3<float>;
using Vec3u = Vec3<unsigned int>;
}  // namespace mep

#include <MEP-3D/vector.inl>

#endif