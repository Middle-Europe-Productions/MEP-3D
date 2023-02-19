#include <MEP-3D/platform/platform_delegate.hpp>

#include <MEP-3D/no_destructor.hpp>
#include <MEP-3D/platform/platform_utils.hpp>

#include <glog/logging.h>

#include <intrin.h>
#include <windows.h>

namespace {
constexpr std::size_t kDivisorKb = 1024;
PlatformDelegate::ProcessorType ToProcessType(WORD type) {
  switch (type) {
    case PROCESSOR_ARCHITECTURE_AMD64:
      return PlatformDelegate::ProcessorType::X64;
    case PROCESSOR_ARCHITECTURE_ARM:
      return PlatformDelegate::ProcessorType::ARM;
    case PROCESSOR_ARCHITECTURE_ARM64:
      return PlatformDelegate::ProcessorType::ARM64;
    case PROCESSOR_ARCHITECTURE_INTEL:
      return PlatformDelegate::ProcessorType::X86;
    case PROCESSOR_ARCHITECTURE_UNKNOWN:
    default:
      return PlatformDelegate::ProcessorType::Unknown;
  }
}
PlatformDelegate::ProcessorType GetProcessorType() {
  static PlatformDelegate::ProcessorType s_type = []() {
    VLOG(3) << "Init processor type";
    SYSTEM_INFO lpSystemInfo;
    GetSystemInfo(&lpSystemInfo);
    return ToProcessType(lpSystemInfo.wProcessorArchitecture);
  }();
  return s_type;
};
std::size_t GetNumberOfCores() {
  static std::size_t s_cores = []() {
    VLOG(3) << "Init number of cores";
    SYSTEM_INFO lpSystemInfo;
    GetSystemInfo(&lpSystemInfo);
    return lpSystemInfo.dwNumberOfProcessors;
  }();
  return s_cores;
};
std::string GetCpuInfo() {
  static std::string s_cpu_info = []() {
    int CPUInfo[4] = {-1};
    unsigned nExIds, i = 0;
    char CPUBrandString[0x40];
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    for (i = 0x80000000; i <= nExIds; ++i) {
      __cpuid(CPUInfo, i);
      if (i == 0x80000002)
        memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
      else if (i == 0x80000003)
        memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
      else if (i == 0x80000004)
        memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }
    LOG(INFO) << CPUBrandString;
    return std::string(CPUBrandString);
  }();
  return s_cpu_info;
}
};  // namespace

class PlatformDelegateWindows : public PlatformDelegate {
 public:
  PlatformDelegateWindows() {
    LOG(INFO) << __func__ << " initialized";
    snapshot_.cpu_info = GetCpuInfo();
    snapshot_.number_of_avalible_cores = GetNumberOfCores();
    snapshot_.processor_type = GetProcessorType();
    utils::FetchGpuInfo(&snapshot_);
  }
  MemorySnapshot GetMemorySnapshot() override;

 private:
  MemorySnapshot snapshot_;
};

PlatformDelegate* PlatformDelegate::Get() {
  static NoDestructor<PlatformDelegateWindows> instance;
  return instance.get();
}

PlatformDelegate::MemorySnapshot PlatformDelegateWindows::GetMemorySnapshot() {
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  GlobalMemoryStatusEx(&statex);
  snapshot_.total_ram_memory_kb = statex.ullTotalPhys / kDivisorKb;
  snapshot_.avalible_ram_memory_kb = statex.ullAvailPhys / kDivisorKb;
  return snapshot_;
}
