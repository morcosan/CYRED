#version 330

layout(location = 0) in vec3 IN_position;
layout(location = 1) in vec4 IN_color;

uniform mat4 DEFAULT_world;
uniform mat4 DEFAULT_view;
uniform mat4 DEFAULT_projection;

out vec4 INTER_vertColor;


void main()
{
	INTER_vertColor = IN_color;
	gl_Position = DEFAULT_projection * DEFAULT_view * DEFAULT_world * vec4( IN_position, 1.0f ); 
}