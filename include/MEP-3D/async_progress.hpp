#ifndef ASYNC_PROGRESS_HPP
#define ASYNC_PROGRESS_HPP

#include <atomic>

class AsyncProgress {
 public:
  AsyncProgress();
  float GetProgress();
  void SetProgress(float progress);

 private:
  std::atomic<float> progress_;
};

#endif