#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 INTER_vertColor[];

out vec4 INTER_fragColor;


void main() 
{
	INTER_fragColor = INTER_vertColor[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	INTER_fragColor = INTER_vertColor[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	INTER_fragColor = INTER_vertColor[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
}