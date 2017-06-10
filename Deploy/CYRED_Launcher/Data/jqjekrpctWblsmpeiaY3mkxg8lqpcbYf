#version 330

layout(location = 0) in vec3 IN_position;
layout(location = 1) in vec4 IN_color;

out vec4 INTER_vertColor;


void main()
{
	INTER_vertColor = IN_color;
	gl_Position = vec4( IN_position, 1.0f ); 
}