#version 330

layout(location = 0) out vec4 OUT_color;

uniform vec4 		textColor;
uniform sampler2D 	textTexture;

in vec2 INTER_fragUV;


void main()
{
	vec4 textureMap = texture( textTexture, INTER_fragUV );
	OUT_color = textColor * textureMap;
}