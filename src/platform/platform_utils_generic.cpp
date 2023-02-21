#include <MEP-3D/platform/platform_utils.hpp>

#include <GL/glew.h>
#include <glog/logging.h>

namespace mep {
namespace {
std::string GetGlString(GLenum gl_enum) {
  const GLubyte* driver = glGetString(gl_enum);
  DCHECK(driver);
  return std::string(reinterpret_cast<const char*>(driver));
}
}  // namespace

namespace utils {
void FetchGpuInfo(PlatformDelegate::MemorySnapshot* memory_snapshot) {
  DCHECK(memory_snapshot);
  memory_snapshot->gpu_driver = GetGlString(GL_VERSION);
  memory_snapshot->gpu_name = GetGlString(GL_RENDERER);
  memory_snapshot->gpu_vendor = GetGlString(GL_VENDOR);
}
}  // namespace utils
}  // namespace mep
