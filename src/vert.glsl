#version 330 core

layout (location = 0) in vec3 uv;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 customColor;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(uv, 1.0);
    customColor = aColor;
    texCoord = aTexCoord;
}
