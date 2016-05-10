#version 330

layout(location = 0) out vec4 OUT_color;

uniform vec4 		diffuseColor;
uniform sampler2D 	diffuseTexture;

in vec2 INTER_fragUV;


void main()
{
	vec4 textureColor = texture( diffuseTexture, INTER_fragUV );
	OUT_color = diffuseColor * textureColor;
}