#ifndef SHADERS_PRESET_HPP
#define SHADERS_PRESET_HPP

#include <MEP-3D/shader.hpp>

namespace mep {
namespace shaders {
extern const char* kBasicVertexShader;
extern const char* kBasicFragmentShader;
void CacheDefaultUnifroms(Shader& shader);

}  // namespace shaders
}  // namespace mep

#endif