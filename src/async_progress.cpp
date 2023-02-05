#include <MEP-3D/async_progress.hpp>

#include <glog/logging.h>

AsyncProgress::AsyncProgress() : progress_(0.f) {}

float AsyncProgress::GetProgress() {
  return progress_.load();
}

void AsyncProgress::SetProgress(float progress) {
  DCHECK(progress >= 0.f && progress <= 1.f);
  progress_ = progress;
}
