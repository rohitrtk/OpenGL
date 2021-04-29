#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D shaderTexture;

void main()
{
	//FragColor = mix(texture(texture0, TexCoord), texture(texture1, vec2(1.0 - TexCoord.x, TexCoord.y)), alpha);
	FragColor = texture(shaderTexture, TexCoord);
}