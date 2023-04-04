
#version 330 core

layout(location = 0) out vec4 FragColor;  // fragment shader output

smooth in vec3 tex_coord;  // 3D texture coordinates form vertex shader
                           // interpolated by rasterizer

// uniforms
uniform sampler3D volume;             // volume dataset
uniform sampler2D transfer_function;  // transfer function
uniform vec3 eye_position;            // camera position
uniform vec3 step_size;               // ray step size

// constants
const int MAX_SAMPLES = 200;  // total samples for each ray march step
const vec3 texMin = vec3(0);  // minimum texture access coordinate
const vec3 texMax = vec3(1);  // maximum texture access coordinate
const float OPACITY_FACTOR = 0.05;

void main() {
  vec3 dataPos = tex_coord;
  vec3 geomDir = normalize((tex_coord - vec3(0.5)) - eye_position);
  vec3 dirStep = geomDir * step_size;
  bool stop = false;
  FragColor = vec4(0.0, 0.0, 0.0, 0.0);

  for (int i = 0; i < MAX_SAMPLES; i++) {
    dataPos = dataPos + dirStep;

    // Ray termination: Test if outside volume ...
    vec3 temp1 = sign(dataPos - texMin);
    vec3 temp2 = sign(texMax - dataPos);
    float inside = dot(temp1, temp2);
    // ... and exit loop
    if (inside < 3.0)
      break;

    float sample = texture(volume, dataPos).r;

    vec4 src = texture(transfer_function, vec2(sample, 0));

    src.a = 1.0 - pow(1.0 - src.a, 0.05);

    FragColor.rgb += (1.0 - FragColor.a) * src.a * src.rgb;

    FragColor.a += (1.0 - FragColor.a) * src.a;

    if (FragColor.a > 0.95)
      break;
  }
}
