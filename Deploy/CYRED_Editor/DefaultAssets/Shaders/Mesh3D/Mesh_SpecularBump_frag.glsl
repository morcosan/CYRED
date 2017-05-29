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

// uniforms
uniform int 		DEFAULT_lightsCount;
uniform Light 		DEFAULT_lights[ MAX_LIGHTS ];
uniform vec3 		DEFAULT_ambientColor;
uniform vec3 		DEFAULT_cameraPosWorld;

uniform vec4 		diffuseColor;
uniform sampler2D 	diffuseTexture;

uniform float 		specularIntensity;
uniform float 		specularPower;
uniform sampler2D 	specularTexture;

uniform sampler2D 	normalTexture;


// varying
in vec3 INTER_fragPosWorld;
in vec3 INTER_fragNormWorld;
in vec2 INTER_fragUV;
in vec3 INTER_fragTanWorld;
in vec3 INTER_fragBitanWorld;


void main()
{
	// prepare vectors
	vec3 normal 		= normalize( INTER_fragNormWorld );
	vec3 tangent 		= normalize( INTER_fragTanWorld );
	vec3 bitangent 		= normalize( INTER_fragBitanWorld );

	// prepare textures
	vec4 textureMap 	= texture( diffuseTexture, INTER_fragUV );
	vec3 specularMap 	= texture( specularTexture, INTER_fragUV ).xyz;
	vec4 normalMap 		= texture( normalTexture, INTER_fragUV );

	// Gramm-Schmidt process
	tangent 			= normalize( tangent - dot(tangent, normal) * normal );
	bitangent 			= normalize( bitangent - dot(bitangent, normal) * normal );
	mat3 TBN 			= mat3(tangent, bitangent, normal);
	vec3 bumpColor 		= 2.0f * normalMap.xyz - 1.0f;
	vec3 bumpNormal 	= normalize( TBN * bumpColor );

	// prepare light
	vec3 lightFactor = vec3(0, 0, 0);
	vec3 specularColor = vec3(0, 0, 0);


	// parse all lights
	for ( int i = 0; i < DEFAULT_lightsCount; i++ ) {
		vec3 rayDirection;
		float diffuseFactor;
		float spotAttenuation; 
		float distanceAttenuation;
		float angleAttenuation;

		// calculate based on light's type
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
			
			diffuseFactor = dot(bumpNormal, - rayDirection);
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

			diffuseFactor = dot(bumpNormal, - rayDirection);
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

		// calculate specular color
		if ( diffuseFactor > 0 ) {
			vec3 vertexToEye = normalize( DEFAULT_cameraPosWorld - INTER_fragPosWorld );
			vec3 reflectDirection = normalize( reflect( rayDirection, normal ) );
			float specularFactor = max( 0, dot( vertexToEye, reflectDirection ) );
			specularFactor = pow(specularFactor, specularPower * 100.0f);

			specularColor += DEFAULT_lights[i].color * 
							DEFAULT_lights[i].intensity *
							specularFactor *
							specularIntensity * 10.0f *
							angleAttenuation *
							distanceAttenuation *
							spotAttenuation;
		}
	}

	// add ambient light
	lightFactor += DEFAULT_ambientColor;
	

	// create final color
	OUT_color = diffuseColor * textureMap * vec4( lightFactor, 1.0f ) 
		 		+ vec4( specularColor * specularMap, 0.0f );

	OUT_color = diffuseColor * textureMap * vec4( lightFactor, 1.0f ) ;
}