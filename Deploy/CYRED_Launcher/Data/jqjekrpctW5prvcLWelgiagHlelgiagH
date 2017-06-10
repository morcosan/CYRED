#version 330

layout(location = 0) in vec3 IN_position;
layout(location = 3) in vec2 IN_uv;

uniform mat4 DEFAULT_world;
uniform mat4 DEFAULT_view;
uniform mat4 DEFAULT_projection;

out vec2 INTER_vertUV;


void main()
{
	INTER_vertUV = IN_uv;
	gl_Position = DEFAULT_projection * DEFAULT_view * DEFAULT_world * vec4( IN_position, 1.0f ); 
}