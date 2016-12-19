// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "../../../../2_BuildingBlocks/Data/DataArray.h"
#include "JsonSerializer.h"
#include "../../../Render/Components/ParticleEmitter.h"


namespace CYRED
{
	class DLL JsonSerializer_ParticleEmitter : public JsonSerializer
	{
		const Char* const ENABLED				= "enabled";

		const Char* const MAX_PARTICLES			= "MaxParticles";
		const Char* const PARTICLE_LIFETIME		= "ParticleLifetime";
		const Char* const PARTICLE_VELOCITY		= "ParticleVelocity";
		const Char* const PARTICLE_ACCEL		= "ParticleAcceleration";
		const Char* const SHAPE_DRIVEN_SPEED	= "ShapeDrivenSpeed";
		const Char* const SHAPE_DRIVEN_ACCEL	= "ShapeDrivenAccel";

		const Char* const WAVES_PER_SEC			= "WavesPerSec";
		const Char* const PARTICLES_PER_WAVE	= "ParticlesPerWave";
		const Char* const LOOPING				= "Looping";
		const Char* const SPAWN_DURATION		= "SpawnDuration";

		const Char* const EMITTER_SHAPE			= "EmitterShape";
		const Char* const SHAPE_RADIUS			= "ShapeRadius";
		const Char* const SPAWN_FROM_EDGE		= "SpawnFromEdge";

		const Char* const MATERIAL				= "Material";
		const Char* const PARTICLE_SIZE_START	= "Particle_Size_Start";
		const Char* const PARTICLE_SIZE_END		= "Particle_Size_End";


	public:
		JsonSerializer_ParticleEmitter();
		virtual ~JsonSerializer_ParticleEmitter() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;


	protected:
		DataArray<EmitterShape>	_shapeEnums;
		DataArray<String>	_shapeStrings;
	};
}