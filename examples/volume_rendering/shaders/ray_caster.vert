#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

smooth out vec3 tex_coord;

void main()
{
	gl_Position = projection*view*model*vec4(pos.xyz,1);
	tex_coord = pos + vec3(0.5);
}
