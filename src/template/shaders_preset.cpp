#include <MEP-3D/common.hpp>
#include <MEP-3D/template/shaders_preset.hpp>

namespace mep {
namespace shaders {
const char* kBasicVertexShader = R"(
#version 330
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

out vec4 local_color;
out vec3 normal;
out vec3 fragment_position;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
  gl_Position = projection * view * model * vec4(pos, 1.0);
  local_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0);

  normal = mat3(transpose(inverse(model))) * norm;

  fragment_position = (model * vec4(pos, 1.0)).xyz;
})";

const char* kBasicFragmentShader = R"(
#version 330

in vec4 local_color;
 
out vec4 color;

void main()
{
    color = local_color;
}
)";

void CacheDefaultUnifroms(Shader& shader) {
  shader.Cache("projection",
               static_cast<unsigned int>(CommonUniform::Projection));
  shader.Cache("view", static_cast<unsigned int>(CommonUniform::View));
  shader.Cache("model", static_cast<unsigned int>(CommonUniform::Model));
  shader.Cache("eye_position",
               static_cast<unsigned int>(CommonUniform::Position));
}
}  // namespace shaders
}  // namespace mep
