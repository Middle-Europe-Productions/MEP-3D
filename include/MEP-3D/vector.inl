
template<typename T>
constexpr Vec2<T>::Vec2(): x_(0), y_(0) {}

template<typename T>
constexpr Vec2<T>::Vec2(const T& x, const T& y): x_(x), y_(y) {}

template<typename T>
template<typename U>
constexpr Vec2<T>::Vec2(const Vec2<U>& u) {
    x_ = static_cast<T>(u.x_);
    y_ = static_cast<T>(u.y_);
}

template<typename T>
constexpr Vec2<T>& Vec2<T>::operator-(const Vec2& x) const {
    x_ -= x.x_;
    y_ -= y.y_;
    return *this;
}

template<typename T>
constexpr Vec2<T>& Vec2<T>::operator+(const Vec2& x) const {
    x_ += x.x_;
    y_ += y.y_;
    return *this;
}

template<typename T>
constexpr bool Vec2<T>::operator==(const Vec2& x) const {
    return x_ == x.x_ && y_ == y.y_;
}
