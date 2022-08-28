#version 330                                                                  

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 local_color;
out vec2 texture_coordinates;
out vec3 normal;
out vec3 fragment_position;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
    local_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0);

    texture_coordinates = tex;

    //Normal only changes in the case of scale and rotation
    //We ignore the possition
    //transpose/inverse -> Protection for the non uniform scaling
    normal = mat3(transpose(inverse(model))) * norm;

    fragment_position = (model * vec4(pos, 1.0)).xyz;
}