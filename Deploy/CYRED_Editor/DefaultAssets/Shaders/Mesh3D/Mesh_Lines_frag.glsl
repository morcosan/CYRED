#version 330

layout(location = 0) out vec4 OUT_color;

in vec4 INTER_fragColor;


void main()
{
	OUT_color = INTER_fragColor;
}