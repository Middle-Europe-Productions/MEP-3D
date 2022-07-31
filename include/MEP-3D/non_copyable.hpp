#ifndef NON_COPYABLE_HPP
#define NON_COPYABLE_HPP

class NonCopyable {
 public:
  NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable(NonCopyable&&) = delete;
  auto operator=(const NonCopyable&) = delete;
  auto operator=(NonCopyable&&) = delete;
};

#endif