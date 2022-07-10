#version 330                                                                  

layout (location = 0) in vec3 pos;
out vec4 local_color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    if (model != 0) gl_Position = model * vec4(pos, 1.0);
    if (view != 0) gl_Position = view * gl_Position;
    if (projection != 0) gl_Position = projection * gl_Position;
    local_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0);
}