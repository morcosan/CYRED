#version 330

layout(location = 0) in vec3 IN_position;
layout(location = 2) in vec3 IN_normal;

uniform mat4 DEFAULT_world;
uniform mat4 DEFAULT_view;
uniform mat4 DEFAULT_projection;

out vec3 INTER_vertPosWorld;
out vec3 INTER_vertNormWorld;


void main()
{
	INTER_vertPosWorld 	= ( DEFAULT_world * vec4( IN_position, 1.0 ) ).xyz;
	INTER_vertNormWorld = ( DEFAULT_world * vec4( IN_normal, 0.0 ) ).xyz;

	gl_Position = DEFAULT_projection * DEFAULT_view * DEFAULT_world * vec4( IN_position, 1.0f ); 
}