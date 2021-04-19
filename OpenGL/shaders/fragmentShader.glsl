#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform vec3 colourOffset;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    //FragColor = texture(texture1, TexCoord) * vec4(ourColor.x + colourOffset.x, ourColor.y + colourOffset.y, ourColor.z + colourOffset.z, 1.0f);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}