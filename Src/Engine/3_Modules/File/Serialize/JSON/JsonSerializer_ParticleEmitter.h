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

		const Char* const MAX_PARTICLES			= "max_particles";
		const Char* const PARTICLE_LIFETIME		= "particle_lifetime";
		const Char* const PARTICLE_VELOCITY		= "particle_velocity";
		const Char* const PARTICLE_ACCEL		= "particle_acceleration";
		const Char* const SHAPE_DRIVEN_SPEED	= "shape_driven_speed";
		const Char* const SHAPE_DRIVEN_ACCEL	= "shape_driven_accel";

		const Char* const WAVES_PER_SEC			= "waves_per_sec";
		const Char* const PARTICLES_PER_WAVE	= "particles_per_wave";
		const Char* const LOOPING				= "looping";
		const Char* const SPAWN_DURATION		= "spawn_duration";

		const Char* const EMITTER_SHAPE			= "emitter_shape";
		const Char* const SHAPE_RADIUS			= "shape_radius";
		const Char* const SPAWN_FROM_EDGE		= "epawn_from_edge";

		const Char* const MATERIAL				= "material";
		const Char* const PARTICLE_SIZE_START	= "particle_size_start";
		const Char* const PARTICLE_SIZE_END		= "particle_size_end";


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