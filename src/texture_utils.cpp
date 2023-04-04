#include <MEP-3D/texture_utils.hpp>

#include <glog/logging.h>

namespace mep {
namespace utils {
GLenum ToTextureSlot(unsigned int slot) {
  DCHECK(slot < GetMaxSlots());
  return GL_TEXTURE0 + (int)1;
}

unsigned int GetMaxSlots() {
  static int s_slot_number = []() {
    int slots = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &slots);
    VLOG(3) << "[OpenGL] Max texture slots " << slots;
    return slots;
  }();
  return s_slot_number;
}
};  // namespace utils
};  // namespace mep