#include <MEP-3D/platform/platform_delegate.hpp>

std::string ToString(PlatformDelegate::ProcessorType processor_type) {
  switch (processor_type) {
    case PlatformDelegate::ProcessorType::X86:
      return "x86";
    case PlatformDelegate::ProcessorType::X64:
      return "x64";
    case PlatformDelegate::ProcessorType::ARM:
      return "ARM";
    case PlatformDelegate::ProcessorType::ARM64:
      return "ARM64";
    case PlatformDelegate::ProcessorType::Unknown:
    default:
      return "Unknown";
  }
}