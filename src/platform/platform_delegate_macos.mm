#include <MEP-3D/platform/platform_delegate.hpp>

#include <MEP-3D/no_destructor.hpp>
#include <MEP-3D/platform/platform_utils.hpp>

#include <glog/logging.h>

#import <Cocoa/Cocoa.h>
#import <IOKit/graphics/IOGraphicsLib.h>

namespace mep {
class PlatformDelegateMacos : public PlatformDelegate {
 public:
  PlatformDelegateMacos() {
    LOG(INFO) << __func__ << " initialized";
    utils::FetchGpuInfo(&snapshot_);
  }
  MemorySnapshot GetMemorySnapshot() override;
  std::filesystem::path OpenFile(Window* window, FileFilter filter) override {
    DCHECK(window);
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    [panel setCanChooseDirectories:NO];
    [panel setAllowsMultipleSelection:NO];
    [panel setCanChooseFiles:YES];
    [panel setMessage: [NSString stringWithUTF8String:filter.title.data()]];
    if (!filter.extensions.empty()) {
      NSMutableArray* typesArray =
          [NSMutableArray arrayWithCapacity:filter.extensions.size()];
      for (const auto& ext : filter.extensions)
        [typesArray addObject:[NSString stringWithUTF8String:ext.data()]];

      [panel setAllowedFileTypes:typesArray];
    }

    NSInteger resultCode = [panel runModal];
    if (resultCode == NSFileHandlingPanelOKButton) {
      NSString* result = [[[panel URLs] firstObject] path];
      if (result) {
        VLOG(3) << "Detected path " << [result UTF8String];
        return std::filesystem::path([result UTF8String]);
      }
    }
    return std::filesystem::path();
  }

  std::filesystem::path SaveFile(Window*, FileFilter) override {
    return std::filesystem::path();
  }

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
