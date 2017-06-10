#version 430
// Particles.frag

layout(location = 0) out vec4 OUT_color;

uniform sampler2D 	particleTexture;

in vec2 INTER_fragUV;
in vec4 INTER_fragColor;


void main()
{
	vec4 textureMap = texture( particleTexture, INTER_fragUV );
	OUT_color = INTER_fragColor * textureMap;
}