#version 430

struct Particle
{
	vec4 position;
	vec4 velocity;
	vec4 acceleration;
	vec4 age_sizeS_sizeE;
};

layout(std140, binding = 0) buffer Particles
{
	Particle data[];
};


uniform mat4 	DEFAULT_world;

uniform int 	isLooping;
uniform float 	deltaTime;
uniform float 	lifetime;

uniform vec4 	colorStart;
uniform vec4 	colorEnd;

out float 	INTER_age;
out float 	INTER_size;
out vec4 	INTER_color;


void main()
{
	float age 	= data[gl_VertexID].age_sizeS_sizeE.x;
	vec3 pos 	= data[gl_VertexID].position.xyz;
	vec3 vel 	= data[gl_VertexID].velocity.xyz;
	vec3 acc 	= data[gl_VertexID].acceleration.xyz;

	vel = vel + acc * age;
	pos = pos + vel * age;

	float sizeStart = data[gl_VertexID].age_sizeS_sizeE.y;
	float sizeEnd = data[gl_VertexID].age_sizeS_sizeE.z;

	//! send to geometry
	INTER_age = age;
	INTER_size = mix( sizeStart, sizeEnd, age / lifetime );
	INTER_color = mix( colorStart, colorEnd, age / lifetime );

	gl_Position = DEFAULT_world * vec4( pos, 1.0f ); 

	//! particles start with negative age until they are born
	age += deltaTime;
	if ( age >= lifetime && isLooping == 1 )
	{
		age = age - lifetime;
	}

	//! store back new values
	data[gl_VertexID].age_sizeS_sizeE.x = age;
}