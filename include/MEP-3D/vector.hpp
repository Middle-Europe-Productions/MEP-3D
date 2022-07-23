#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string.h>

template <typename T>
class Vec2 {
 public:
  constexpr Vec2();
  constexpr Vec2(const T& x, const T& y);
  template <typename U>
  constexpr Vec2(const Vec2<U>& u);
  constexpr Vec2<T>& operator-(const Vec2& x) const;
  constexpr Vec2<T>& operator+(const Vec2& x) const;
  constexpr bool operator==(const Vec2& x) const;
  T x_;
  T y_;
};
using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec2u = Vec2<unsigned int>;

template <typename T>
class Vec3 : public Vec2<T> {
 public:
  constexpr Vec3();
  constexpr Vec3(const T& x, const T& y, const T& z);
  template <typename U>
  constexpr Vec3(const Vec3<U>& u);
  constexpr Vec3<T>& operator-(const Vec3& x) const;
  constexpr Vec3<T>& operator+(const Vec3& x) const;
  constexpr bool operator==(const Vec3& x) const;
  T z_;
};

using Vec3i = Vec3<int>;
using Vec3f = Vec3<float>;
using Vec3u = Vec3<unsigned int>;

#include <MEP-3D/vector.inl>

#endif