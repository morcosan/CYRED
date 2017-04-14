#version 330

layout(location = 0) in vec3 IN_posFirst;
layout(location = 1) in vec3 IN_posSecond;
layout(location = 4) in vec2 IN_uv;

uniform mat4 	DEFAULT_world;
uniform mat4 	DEFAULT_view;
uniform mat4 	DEFAULT_projection;

uniform float 	stateRatio;

out vec2 INTER_vertUV;


void main()
{
	INTER_vertUV = IN_uv;

	vec3 pos = mix( IN_posFirst, IN_posSecond, stateRatio );

	gl_Position = DEFAULT_projection * DEFAULT_view * DEFAULT_world * vec4( pos, 1.0f ); 
}