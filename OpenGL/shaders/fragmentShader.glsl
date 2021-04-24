#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform float alpha;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	FragColor = mix(texture(texture0, TexCoord), texture(texture1, vec2(1.0 - TexCoord.x, TexCoord.y)), alpha);
	//FragColor = texture(texture0, TexCoord);
}