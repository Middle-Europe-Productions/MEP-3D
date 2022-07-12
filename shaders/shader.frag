#version 330

in vec4 local_color;
in vec2 texture_coordinates;

out vec4 colour;

uniform sampler2D texture_;

void main()
{
    colour = texture(texture_, texture_coordinates) * local_color;
}