#version 330

const int MAX_LIGHTS = 8;   


layout(location = 0) out vec4 OUT_color;


struct Light 
{
	int 	type;		// 0 = directional, 1 = point, 2 = spot
	vec3 	color;
	float 	intensity;
	float 	spotAngle;
	float 	range;
	vec3 	positionWorld;
	vec3 	directionWorld;
};

uniform int 	DEFAULT_lightsCount;
uniform Light 	DEFAULT_lights[ MAX_LIGHTS ];
uniform vec3 	DEFAULT_ambientColor;

uniform vec4 		diffuseColor;
uniform sampler2D 	diffuseTexture;

in vec3 INTER_fragPosWorld;
in vec3 INTER_fragNormWorld;
in vec2 INTER_fragUV;


void main()
{
	vec3 normal = normalize( INTER_fragNormWorld );

	vec4 textureColor = texture( diffuseTexture, INTER_fragUV );

	vec3 lightFactor = vec3(0, 0, 0);

	for ( int i = 0; i < DEFAULT_lightsCount; i++ ) {
		vec3 rayDirection;
		float diffuseFactor;
		float spotAttenuation; 
		float distanceAttenuation;
		float angleAttenuation;

		if ( DEFAULT_lights[i].type == 0 ) {
			rayDirection = normalize( DEFAULT_lights[i].directionWorld );
			diffuseFactor = dot(normal, - rayDirection);
			angleAttenuation = max(0, diffuseFactor);
			lightFactor += 	DEFAULT_lights[i].color * 
							DEFAULT_lights[i].intensity * 
							angleAttenuation;

			spotAttenuation = 1.0;
			distanceAttenuation = 1.0f;
		}
		else if ( DEFAULT_lights[i].type == 1 )	{
			rayDirection = normalize( INTER_fragPosWorld - DEFAULT_lights[i].positionWorld );
			float distance = length( INTER_fragPosWorld - DEFAULT_lights[i].positionWorld );
			
			diffuseFactor = dot(normal, - rayDirection);
			angleAttenuation = max(0, diffuseFactor);
			float distanceToRange = distance / DEFAULT_lights[i].range;
			distanceAttenuation = max( 0, 1 - distanceToRange / 2 - distanceToRange * distanceToRange / 2 );

			lightFactor += 	DEFAULT_lights[i].color * 
							DEFAULT_lights[i].intensity * 
							angleAttenuation * 
							distanceAttenuation;

			spotAttenuation = 1.0;
		}
		else if ( DEFAULT_lights[i].type == 2 )	{
			rayDirection = normalize( INTER_fragPosWorld - DEFAULT_lights[i].positionWorld );
			float distance = length( INTER_fragPosWorld - DEFAULT_lights[i].positionWorld );

			diffuseFactor = dot(normal, - rayDirection);
			angleAttenuation = max(0, diffuseFactor);
			float distanceToRange = distance / DEFAULT_lights[i].range;
			distanceAttenuation = max( 0, 1 - distanceToRange / 2 - distanceToRange * distanceToRange / 2 );

			float spotFactor = dot( rayDirection, DEFAULT_lights[i].directionWorld );
			float spotCutoff = cos( DEFAULT_lights[i].spotAngle / 2 );

			if (spotFactor < spotCutoff)	continue;

            spotAttenuation = max( 0, 1.0 - (1.0 - spotFactor) / (1.0 - spotCutoff) );
           
           	lightFactor += 	DEFAULT_lights[i].color * 
           					DEFAULT_lights[i].intensity * 
           					angleAttenuation * 
           					distanceAttenuation *
           					spotAttenuation;
		}
	}

    lightFactor += DEFAULT_ambientColor;
	

	OUT_color = diffuseColor * textureColor * vec4( lightFactor, 1.0f );
}