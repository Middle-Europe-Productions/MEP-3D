#ifndef PLATFORM_UTILS_HPP
#define PLATFORM_UTILS_HPP

#include <MEP-3D/platform/platform_delegate.hpp>

namespace mep {
namespace utils {
void FetchGpuInfo(PlatformDelegate::MemorySnapshot* memory_snapshot);
};  // namespace utils
};  // namespace mep
#endif
