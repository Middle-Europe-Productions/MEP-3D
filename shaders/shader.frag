#version 330

in vec4 local_color;
in vec2 texture_coordinates;

out vec4 colour;

uniform sampler2D texture_;
uniform int use_texture;

void main()
{
    if(use_texture == 1)
        colour = texture(texture_, texture_coordinates);
    else
        colour = local_color;
}