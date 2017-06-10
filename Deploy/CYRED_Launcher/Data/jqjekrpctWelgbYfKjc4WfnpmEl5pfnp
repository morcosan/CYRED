#version 330

layout(location = 0) in vec3 IN_posFirst;
layout(location = 1) in vec3 IN_posSecond;
layout(location = 3) in vec3 IN_normalFirst;
layout(location = 4) in vec3 IN_normalSecond;

uniform mat4 	DEFAULT_world;
uniform mat4 	DEFAULT_view;
uniform mat4 	DEFAULT_projection;

uniform float 	stateRatio;

out vec3 INTER_vertPosWorld;
out vec3 INTER_vertNormWorld;


void main()
{
	vec3 pos = mix( IN_posFirst, IN_posSecond, stateRatio );
	vec3 normal = mix( IN_normalFirst, IN_normalSecond, stateRatio );

	INTER_vertPosWorld = ( DEFAULT_world * vec4( pos, 1.0 ) ).xyz;
	INTER_vertNormWorld = ( DEFAULT_world * vec4( normal, 0.0 ) ).xyz;

	gl_Position = DEFAULT_projection * DEFAULT_view * DEFAULT_world * vec4( pos, 1.0f ); 
}