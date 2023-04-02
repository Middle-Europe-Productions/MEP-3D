#ifndef TEXTURE_UTILS_HPP
#define TEXTURE_UTILS_HPP

#include <GL/glew.h>

namespace mep {
namespace utils {
GLenum ToTextureSlot(unsigned int slot);
unsigned int GetMaxSlots();
};  // namespace utils
};  // namespace mep

#endif