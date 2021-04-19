#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform vec3 positionOffset;

void main()
{
    gl_Position = vec4(aPos.x + positionOffset.x, aPos.y + positionOffset.y, aPos.z + positionOffset.z, 1.0);
    ourColor = aColor;
}