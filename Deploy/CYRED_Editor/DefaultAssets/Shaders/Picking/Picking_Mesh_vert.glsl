#version 330

layout(location = 0) in vec3 IN_position;

uniform mat4 DEFAULT_world;
uniform mat4 DEFAULT_view;
uniform mat4 DEFAULT_projection;


void main()
{
	gl_Position = DEFAULT_projection * DEFAULT_view * DEFAULT_world * vec4( IN_position, 1.0f ); 
}