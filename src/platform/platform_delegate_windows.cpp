#include <MEP-3D/platform/platform_delegate.hpp>

#include <MEP-3D/no_destructor.hpp>
#include <MEP-3D/platform/platform_utils.hpp>

#include <glog/logging.h>

#include <intrin.h>
#include <windows.h>
#include <codecvt>

#ifdef MEP_USES_GLFW
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

namespace mep {
namespace {
constexpr std::size_t kDivisorKb = 1024;
constexpr int kMaxFileNameSize = 256;
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

HWND GetWindowHandle(void* native_window) {
#ifdef MEP_USES_GLFW
  VLOG(1) << __func__ << " requested WIN32 - GLFW instance";
  return glfwGetWin32Window((GLFWwindow*)native_window);
#endif
  VLOG(1) << __func__ << " requested unknown instance";
  return NULL;
}

std::wstring GetRawFilter(const FileFilter filter) {
  std::wstring wide_string =
      std::wstring(filter.title.begin(), filter.title.end());
  for (const auto& ext : filter.extensions) {
    wide_string.push_back(L'\0');
    wide_string.push_back(L'*');
    wide_string.push_back(L'.');
    for (char ele : ext) {
      wide_string.push_back(static_cast<wchar_t>(ele));
    }
  }
  wide_string.push_back(L'\0');
  return wide_string;
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
  std::filesystem::path OpenFile(Window* window, FileFilter filter) override {
    DCHECK(window);
    OPENFILENAMEW ofn;
    WCHAR file[kMaxFileNameSize] = {0};
    auto raw_filter = GetRawFilter(filter);
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = GetWindowHandle(window->GetNativeWindow());
    ofn.lpstrFile = file;
    ofn.nMaxFile = sizeof(file);
    ofn.lpstrFilter = raw_filter.c_str();
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (GetOpenFileNameW(&ofn) == TRUE) {
      return ofn.lpstrFile;
    }
    return std::filesystem::path();
  }

  std::filesystem::path SaveFile(Window* window, FileFilter filter) override {
    return std::filesystem::path();
  }

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
}  // namespace mep
