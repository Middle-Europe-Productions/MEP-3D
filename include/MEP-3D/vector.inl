namespace mep {
template <typename T>
constexpr Vec2<T>::Vec2() : x_(0), y_(0) {}

template <typename T>
constexpr Vec2<T>::Vec2(const T& x, const T& y) : x_(x), y_(y) {}

template <typename T>
template <typename U>
constexpr Vec2<T>::Vec2(const Vec2<U>& u) {
  x_ = static_cast<T>(u.x_);
  y_ = static_cast<T>(u.y_);
}

template <typename T>
constexpr Vec2<T>& Vec2<T>::operator-(const Vec2& x) {
  x_ -= x.x_;
  y_ -= x.y_;
  return *this;
}

template <typename T>
constexpr Vec2<T>& Vec2<T>::operator+(const Vec2& x) {
  x_ += x.x_;
  y_ += x.y_;
  return *this;
}

template <typename T>
constexpr Vec2<T>& Vec2<T>::operator*(const Vec2& x) {
  x_ *= x.x_;
  y_ *= x.y_;
  return *this;
}

template <typename T>
constexpr Vec2<T>& Vec2<T>::operator/(const Vec2& x) {
  x_ /= x.x_;
  y_ /= x.y_;
  return *this;
}

template <typename T>
constexpr bool Vec2<T>::operator==(const Vec2& x) const {
  return x_ == x.x_ && y_ == x.y_;
}

template <typename T>
constexpr T Vec2<T>::Length() const {
  return std::sqrt(x_ * x_ + y_ * y_);
}

template <typename T>
constexpr Vec3<T>::Vec3() : x_(0), y_(0), z_(0) {}

template <typename T>
constexpr Vec3<T>::Vec3(const T& x, const T& y, const T& z)
    : x_(x), y_(y), z_(z) {}

template <typename T>
template <typename U>
constexpr Vec3<T>::Vec3(const Vec3<U>& u) {
  x_ = static_cast<T>(u.x_);
  y_ = static_cast<T>(u.y_);
  z_ = static_cast<T>(u.z_);
}

template <typename T>
constexpr Vec3<T>& Vec3<T>::operator-(const Vec3& x) {
  x_ -= x.x_;
  y_ -= x.y_;
  z_ -= x.z_;
  return *this;
}

template <typename T>
constexpr Vec3<T>& Vec3<T>::operator+(const Vec3& x) {
  x_ += x.x_;
  y_ += x.y_;
  z_ += x.z_;
  return *this;
}

template <typename T>
constexpr Vec3<T>& Vec3<T>::operator*(const Vec3& x) {
  x_ *= x.x_;
  y_ *= x.y_;
  z_ *= x.z_;
  return *this;
}

template <typename T>
constexpr Vec3<T>& Vec3<T>::operator/(const Vec3& x) {
  x_ /= x.x_;
  y_ /= x.y_;
  z_ /= x.z_;
  return *this;
}

template <typename T>
constexpr bool Vec3<T>::operator==(const Vec3& x) const {
  return x_ == x.x_ && y_ == x.y_ && z_ == x.z_;
}

template <typename T>
constexpr T Vec3<T>::Length() const {
  return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

template <typename T>
constexpr T Vec3<T>::Max() const {
  return (std::max)({x_, y_, z_});
}

template <typename T>
constexpr T Vec3<T>::Min() const {}

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vec2<T>& in) {
  out << "\"Vec2\": { \"x\": " << in.x_ << ", \"y\":" << in.y_ << "}";
  return out;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vec3<T>& in) {
  out << "\"Vec3\": { \"x\": " << in.x_ << ", \"y\":" << in.y_
      << ", \"x\":" << in.z_ << "}";
  return out;
}

}  // namespace mep
