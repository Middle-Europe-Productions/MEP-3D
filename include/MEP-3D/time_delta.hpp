#ifndef TIME_DELTA_HPP
#define TIME_DELTA_HPP

#include <memory>

class TimeDelta {
 public:
  virtual double GetCurrentTime() = 0;
  virtual double GetTimeDelta() = 0;
  static std::unique_ptr<TimeDelta> GetInstance();
  virtual ~TimeDelta() = default;
};
using TimeDeltaPtr = std::unique_ptr<TimeDelta>;

#endif