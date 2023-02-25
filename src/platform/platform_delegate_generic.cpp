#include <MEP-3D/platform/platform_delegate.hpp>

#include <MEP-3D/no_destructor.hpp>

#include <glog/logging.h>

namespace mep {
class PlatformDelegateGeneric : public PlatformDelegate {
 public:
  PlatformDelegateGeneric() { LOG(INFO) << __func__ << " initialized"; }
  MemorySnapshot GetMemorySnapshot() override;
  std::filesystem::path OpenFile(Window* window, FileFilter filter) override {}
  std::filesystem::path SaveFile(Window* window, FileFilter filter) override {}
};

PlatformDelegate* PlatformDelegate::Get() {
  static NoDestructor<PlatformDelegateGeneric> instance;
  return instance.get();
}

PlatformDelegate::MemorySnapshot PlatformDelegateGeneric::GetMemorySnapshot() {
  return {"", 0, 0, 0, ProcessorType::Unknown};
}
}  // namespace mep
