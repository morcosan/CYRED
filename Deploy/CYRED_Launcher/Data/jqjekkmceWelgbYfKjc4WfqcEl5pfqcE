#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 INTER_vertPosWorld[];
in vec3 INTER_vertNormalWorld[];

out vec3 INTER_fragPosWorld;
out vec3 INTER_fragNormWorld;


void main() 
{
	INTER_fragPosWorld 		= INTER_vertPosWorld[0];
	INTER_fragNormWorld 	= INTER_vertNormalWorld[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	INTER_fragPosWorld 		= INTER_vertPosWorld[1];
	INTER_fragNormWorld 	= INTER_vertNormalWorld[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	INTER_fragPosWorld 		= INTER_vertPosWorld[2];
	INTER_fragNormWorld 	= INTER_vertNormalWorld[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
}