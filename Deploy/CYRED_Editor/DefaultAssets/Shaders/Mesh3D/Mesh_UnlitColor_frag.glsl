#version 330

layout(location = 0) out vec4 OUT_color;

uniform vec4 diffuseColor;


void main()
{
	OUT_color = diffuseColor;
}