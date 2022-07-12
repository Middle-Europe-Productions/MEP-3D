#version 330                                                                  

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec4 local_color;
out vec2 texture_coordinates;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    if (model != 0) gl_Position = model * vec4(pos, 1.0);
    if (view != 0) gl_Position = view * gl_Position;
    if (projection != 0) gl_Position = projection * gl_Position;
    local_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0);

    texture_coordinates = tex;
}