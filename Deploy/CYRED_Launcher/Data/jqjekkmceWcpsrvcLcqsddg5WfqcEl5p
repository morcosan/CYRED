#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 INTER_vertPosWorld[];
in vec3 INTER_vertNormWorld[];
in vec2 INTER_vertUV[];

out vec3 INTER_fragPosWorld;
out vec3 INTER_fragNormWorld;
out vec2 INTER_fragUV;


void main() 
{
	INTER_fragPosWorld 	= INTER_vertPosWorld[0];
	INTER_fragNormWorld = INTER_vertNormWorld[0];
	INTER_fragUV 		= INTER_vertUV[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	INTER_fragPosWorld 	= INTER_vertPosWorld[1];
	INTER_fragNormWorld = INTER_vertNormWorld[1];
	INTER_fragUV 		= INTER_vertUV[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	INTER_fragPosWorld 	= INTER_vertPosWorld[2];
	INTER_fragNormWorld = INTER_vertNormWorld[2];
	INTER_fragUV 		= INTER_vertUV[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
}