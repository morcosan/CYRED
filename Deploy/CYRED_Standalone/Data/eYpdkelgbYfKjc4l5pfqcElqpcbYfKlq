#version 330

layout(location = 0) out vec4 OUT_color;

uniform vec3 DEFAULT_cameraPosWorld;

uniform vec4 diffuseColor;
uniform vec2 intensityLimits;
uniform vec2 colorLimits;

in vec3 INTER_fragPosWorld;
in vec3 INTER_fragNormalWorld;


void main()
{
	vec3 dirToCamera = normalize( DEFAULT_cameraPosWorld - INTER_fragPosWorld );

	float metallic = dot( INTER_fragNormalWorld, dirToCamera );
	// smooth interpolation between values
	metallic = smoothstep( intensityLimits.x, intensityLimits.y, metallic ); 
	// shift matallic value from range [0, 1] to range[0.5, 1]
	metallic = metallic * (colorLimits.y - colorLimits.x) + colorLimits.x;

	OUT_color = vec4( metallic * diffuseColor.xyz, diffuseColor.w);
}