#ifndef TIME_DELTA_HPP
#define TIME_DELTA_FPP

#include<memory>

class TimeDelta {
public:
	virtual double GetTimeDelta() = 0;
	static std::unique_ptr<TimeDelta> GetInstance();
};
using TimeDeltaPtr = std::unique_ptr<TimeDelta>;

#endif