#version 330 core

out vec4 FragColor;

in vec3 ourColor;

uniform vec3 colourOffset;

void main()
{
    FragColor = vec4(ourColor.x + colourOffset.x, ourColor.y + colourOffset.y, ourColor.z + colourOffset.z, 1.0f);
}