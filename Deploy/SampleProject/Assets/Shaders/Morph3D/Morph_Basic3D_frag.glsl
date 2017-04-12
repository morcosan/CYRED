#version 330

layout(location = 0) out vec4 OUT_color;

in vec2 INTER_fragUV;


void main()
{
	OUT_color = vec4( INTER_fragUV.x, INTER_fragUV.y, 0, 1 );
}