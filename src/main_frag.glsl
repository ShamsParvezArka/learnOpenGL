#version 330 core

in vec3 gradient;
in vec2 texture_pos;

uniform sampler2D _our_texture;

out vec4 FragColor;

void main()
{
   FragColor = texture(_our_texture, texture_pos);
}