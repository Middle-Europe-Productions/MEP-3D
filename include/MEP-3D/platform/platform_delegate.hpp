#ifndef PLATFORM_DELEGATE_HPP
#define PLATFORM_DELEGATE_HPP

#include <MEP-3D/non_copyable.hpp>
#include <MEP-3D/window.hpp>

#include <filesystem>
#include <string>

class PlatformDelegate : public NonCopyable {
 public:
  enum class ProcessorType { X86, X64, ARM, ARM64, Unknown };
  struct MemorySnapshot {
    std::string cpu_info;
    std::size_t total_ram_memory_kb;
    std::size_t avalible_ram_memory_kb;
    std::size_t number_of_avalible_cores;
    ProcessorType processor_type;
    std::string gpu_vendor;
    std::string gpu_name;
    std::string gpu_driver;
  };
  virtual MemorySnapshot GetMemorySnapshot() = 0;
  virtual std::filesystem::path OpenFile(Window* window,
                                         std::wstring filter) = 0;
  virtual std::filesystem::path SaveFile(Window* window,
                                         std::wstring filter) = 0;

  static PlatformDelegate* Get();
};

std::string ToString(PlatformDelegate::ProcessorType processor_type);

#endif
