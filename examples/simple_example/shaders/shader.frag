#version 330

in vec4 local_color;
in vec2 texture_coordinates;
in vec3 normal;
in vec3 fragment_position;

out vec4 color;

const int MAX_POINT_LIGHT = 16;

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
uniform int point_light_count;
uniform DirectionalLight directional_light;
uniform PointLight point_light[MAX_POINT_LIGHT];
uniform Material material;
uniform vec3 eye_position;

vec4 CalculateLightByDirection(Light light, vec3 direction) {
    vec4 ambient_color = vec4(light.color, 1.0f) * light.ambient_intensity;

    //A.B = |A||B|cosA -> 1*1*cosA -> A.B = cosA;
    float diffuse_factor = max(dot(normalize(normal), normalize(direction)), 0.0f);
    vec4 diffuse_color = vec4(light.color * light.diffuse_intensity * diffuse_factor, 1.0f);

    vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);

    if(diffuse_factor > 0.0f && light.diffuse_intensity > 0.0f) {
        vec3 frag_to_eye = normalize(eye_position - fragment_position);
        vec3 reflacted_vertex = normalize(reflect(direction, normalize(normal)));

        float specular_factor = dot(frag_to_eye, reflacted_vertex);
        if (specular_factor > 0.0f) {
            specular_factor = pow(specular_factor, material.shininess);
            specular_color = vec4(light.color * material.specular_intensity * specular_factor, 1.0f);
        }
    }
    return (ambient_color + diffuse_color + specular_color);
}

vec4 CalculatePointLights() {
    vec4 point_light_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < point_light_count; i++) {
        vec3 direction = fragment_position - point_light[i].position;
        float distance = length(direction);
        direction = normalize(direction);

        vec4 layer_color = CalculateLightByDirection(point_light[i].base, direction);
        float attenuation = point_light[i].quadratic * distance * distance +
                            point_light[i].linear * distance +
                            point_light[i].constant;
        point_light_color += (layer_color / attenuation);
    }
    return point_light_color;
}

void main()
{
    //Directional light
    vec4 final_color = CalculateLightByDirection(directional_light.base, directional_light.direction);
    final_color += CalculatePointLights();

    if(use_texture == 1)
        color = texture(texture_, texture_coordinates) * final_color;
    else
        color = local_color * final_color;
}