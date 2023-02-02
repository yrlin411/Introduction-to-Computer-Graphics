#version 330 core

uniform sampler2D texture;

in vec4 color;
out vec4 fragColor;

void main()
{
	fragColor = color;
} 