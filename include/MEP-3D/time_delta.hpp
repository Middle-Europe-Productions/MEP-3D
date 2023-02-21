#ifndef TIME_DELTA_HPP
#define TIME_DELTA_HPP

#include <memory>

namespace mep {
class TimeDelta {
 public:
  virtual float GetCurrentTime() = 0;
  virtual float GetTimeDelta() = 0;
  static std::unique_ptr<TimeDelta> GetInstance();
  virtual ~TimeDelta() = default;
};
using TimeDeltaPtr = std::unique_ptr<TimeDelta>;
}  // namespace mep

#endif