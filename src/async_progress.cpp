#include <MEP-3D/async_progress.hpp>

#include <glog/logging.h>

namespace mep {
AsyncProgress::AsyncProgress() : progress_(0.f) {}

float AsyncProgress::GetProgress() {
  return progress_.load(std::memory_order_acquire);
}

void AsyncProgress::SetProgress(float progress) {
  DCHECK(progress >= 0.f && progress <= 1.f);
  progress_.store(progress, std::memory_order_release);
}
}  // namespace mep
