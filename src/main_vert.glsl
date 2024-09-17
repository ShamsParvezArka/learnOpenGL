#version 330 core

layout (location = 0) in vec3 uv;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 in_texture_pos;

uniform mat4 _transform;

out vec3 gradient;
out vec2 texture_pos;

void main()
{
    gl_Position = _transform * vec4(uv, 1.0);
    gradient = color;
    texture_pos = in_texture_pos;
}
