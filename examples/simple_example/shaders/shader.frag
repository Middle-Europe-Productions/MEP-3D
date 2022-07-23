#version 330

in vec4 local_color;
in vec2 texture_coordinates;
in vec3 normal;
in vec3 fragment_position;

out vec4 color;

const int MAX_NUMBER_OF_POINT_LIGHT = 16;

struct Light {
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
};

struct DirectionalLight {
    Light base;
    vec3 direction;
};

struct PointLight {
    Light base;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct Material {
    float specular_intensity;
    float shininess;
};

uniform sampler2D texture_;
uniform int use_texture;
uniform DirectionalLight directional_light;
uniform PointLight point_light[MAX_NUMBER_OF_POINT_LIGHT];
uniform Material material;
uniform vec3 eye_position;

vec4 CalculateColorByDirection(Light base, vec3 direction) {
    vec4 ambient_color = vec4(base.color, 1.0f) * base.ambient_intensity;

    //A.B = |A||B|cosA -> 1*1*cosA -> A.B = cosA;
    float diffuse_factor = max(dot(normalize(normal), normalize(direction)), 0.0f);
    vec4 diffuse_color = vec4(base.color, 1.0f) * base.diffuse_intensity * diffuse_factor;

    vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);

    if(diffuse_factor > 0) {
        vec3 frag_to_eye = normalize(eye_position - fragment_position);
        vec3 reflacted_vertex = normalize(reflect(direction, normalize(normal)));

        float specular_factor = dot(frag_to_eye, reflacted_vertex);
        if (specular_factor > 0.0f) {
            specular_factor = pow(specular_factor, material.shininess);
            specular_color = vec4(base.color * material.specular_intensity * specular_factor, 1.0f);
        }
    }
    return (ambient_color + diffuse_color + specular_color);
}

void main()
{
    vec4 color = CalculateColorByDirection(directional_light.base, directional_light.direction);

    if(use_texture == 1)
        color = texture(texture_, texture_coordinates) * color;
    else
        color = local_color * color;
}