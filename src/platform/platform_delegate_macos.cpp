#define _POSIX_SOURCE
#include <MEP-3D/platform/platform_delegate.hpp>

#include <MEP-3D/no_destructor.hpp>
#include <MEP-3D/platform/platform_utils.hpp>

#include <glog/logging.h>

// #include <sys/utsname.h>
// #include <sys/sysctl.h>

namespace mep {
class PlatformDelegateMacos : public PlatformDelegate {
 public:
  PlatformDelegateMacos() {
    LOG(INFO) << __func__ << " initialized";
    utils::FetchGpuInfo(&snapshot_);
  }
  MemorySnapshot GetMemorySnapshot() override;

 private:
  MemorySnapshot snapshot_;
};

PlatformDelegate* PlatformDelegate::Get() {
  static NoDestructor<PlatformDelegateMacos> instance;
  return instance.get();
}

PlatformDelegate::MemorySnapshot PlatformDelegateMacos::GetMemorySnapshot() {
  return snapshot_;
}
}  // namespace mep
