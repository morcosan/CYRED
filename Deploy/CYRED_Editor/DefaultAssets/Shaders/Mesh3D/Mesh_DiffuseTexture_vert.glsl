#version 330

layout(location = 0) in vec3 IN_position;
layout(location = 2) in vec3 IN_normal;
layout(location = 3) in vec2 IN_uv;
layout(location = 4) in vec3 IN_tangent;
layout(location = 5) in vec3 IN_bitangent;

uniform mat4 DEFAULT_world;
uniform mat4 DEFAULT_view;
uniform mat4 DEFAULT_projection;

out vec3 INTER_vertPosWorld;
out vec3 INTER_vertNormWorld;
out vec2 INTER_vertUV;
out vec3 INTER_vertTanWorld;
out vec3 INTER_vertBittanWorld;


void main()
{
	INTER_vertPosWorld 		= ( DEFAULT_world * vec4( IN_position, 1.0 ) ).xyz;
	INTER_vertNormWorld 	= ( DEFAULT_world * vec4( IN_normal, 0.0 ) ).xyz;
	INTER_vertUV 			= IN_uv;
	INTER_vertTanWorld 		= ( DEFAULT_world * vec4( IN_tangent, 0.0 ) ).xyz;
	INTER_vertBittanWorld 	= ( DEFAULT_world * vec4( IN_bitangent, 0.0 ) ).xyz;

	gl_Position = DEFAULT_projection * DEFAULT_view * DEFAULT_world * vec4( IN_position, 1.0f ); 
}