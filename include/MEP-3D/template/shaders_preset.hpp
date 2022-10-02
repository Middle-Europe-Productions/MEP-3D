#ifndef SHADERS_PRESET_HPP
#define SHADERS_PRESET_HPP

#include <MEP-3D/shader.hpp>

namespace shaders {
extern char* kBasicVertexShader;
extern char* kBasicFragmentShader;
void CacheDefaultUnifroms(Shader& shader);

}  // namespace shaders

#endif