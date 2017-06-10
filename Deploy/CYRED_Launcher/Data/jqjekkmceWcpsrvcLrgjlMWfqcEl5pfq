#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 INTER_vertUV[];

out vec2 INTER_fragUV;


void main() 
{
	INTER_fragUV = INTER_vertUV[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	INTER_fragUV = INTER_vertUV[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	INTER_fragUV = INTER_vertUV[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
}