#version 330

in vec4 local_color;
in vec2 texture_coordinates;

out vec4 color;

struct DirectionalLight {
    vec3 color;
    float ambient_intensity;
};

uniform sampler2D texture_;
uniform int use_texture;
uniform DirectionalLight directional_light;

void main()
{
    vec4 ambient_color = vec4(directional_light.color, 1.0f) * directional_light.ambient_intensity;

    if(use_texture == 1)
        color = texture(texture_, texture_coordinates) * ambient_color;
    else
        color = local_color * ambient_color;
}