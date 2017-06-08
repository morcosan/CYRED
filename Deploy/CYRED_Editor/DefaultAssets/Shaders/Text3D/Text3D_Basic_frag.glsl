#version 330

layout(location = 0) out vec4 OUT_color;

uniform vec4 		textColor;
uniform sampler2D 	textTexture;

in vec2 INTER_fragUV;


void main()
{
	float alpha = texture(textTexture, INTER_fragUV).r;
	OUT_color = textColor * vec4( 1.0, 1.0, 1.0, alpha );
}