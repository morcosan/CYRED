#version 430

layout(location = 0) out vec4 OUT_color;

uniform sampler2D 	particleTexture;

in vec2 INTER_fragUV;
in vec4 INTER_fragColor;


void main()
{
	vec4 textureColor = texture( particleTexture, INTER_fragUV );
	OUT_color = INTER_fragColor * textureColor;
}