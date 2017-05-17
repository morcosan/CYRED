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
		const char* const ENABLED				= "enabled";

		const char* const MAX_PARTICLES			= "max_particles";
		const char* const PARTICLE_LIFETIME		= "particle_lifetime";
		const char* const PARTICLE_VELOCITY		= "particle_velocity";
		const char* const PARTICLE_ACCEL		= "particle_acceleration";
		const char* const SHAPE_DRIVEN_SPEED	= "shape_driven_speed";
		const char* const SHAPE_DRIVEN_ACCEL	= "shape_driven_accel";

		const char* const WAVES_PER_SEC			= "waves_per_sec";
		const char* const PARTICLES_PER_WAVE	= "particles_per_wave";
		const char* const LOOPING				= "looping";
		const char* const SPAWN_DURATION		= "spawn_duration";

		const char* const EMITTER_SHAPE			= "emitter_shape";
		const char* const SHAPE_RADIUS			= "shape_radius";
		const char* const SPAWN_FROM_EDGE		= "epawn_from_edge";

		const char* const MATERIAL				= "material";
		const char* const PARTICLE_SIZE_START	= "particle_size_start";
		const char* const PARTICLE_SIZE_END		= "particle_size_end";


	public:
		JsonSerializer_ParticleEmitter();
		virtual ~JsonSerializer_ParticleEmitter() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;


	protected:
		DataArray<EmitterShape>	_shapeEnums;
		DataArray<String>	_shapeStrings;
	};
}