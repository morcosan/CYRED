// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		cchar* const ENABLED				= "enabled";

		cchar* const MAX_PARTICLES			= "max_particles";
		cchar* const PARTICLE_LIFETIME		= "particle_lifetime";
		cchar* const PARTICLE_VELOCITY		= "particle_velocity";
		cchar* const PARTICLE_ACCEL		= "particle_acceleration";
		cchar* const SHAPE_DRIVEN_SPEED	= "shape_driven_speed";
		cchar* const SHAPE_DRIVEN_ACCEL	= "shape_driven_accel";

		cchar* const WAVES_PER_SEC			= "waves_per_sec";
		cchar* const PARTICLES_PER_WAVE	= "particles_per_wave";
		cchar* const LOOPING				= "looping";
		cchar* const SPAWN_DURATION		= "spawn_duration";

		cchar* const EMITTER_SHAPE			= "emitter_shape";
		cchar* const SHAPE_RADIUS			= "shape_radius";
		cchar* const SPAWN_FROM_EDGE		= "epawn_from_edge";

		cchar* const MATERIAL				= "material";
		cchar* const PARTICLE_SIZE_START	= "particle_size_start";
		cchar* const PARTICLE_SIZE_END		= "particle_size_end";


	public:
		JsonSerializer_ParticleEmitter();
		virtual ~JsonSerializer_ParticleEmitter() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;


	private:
		DataArray<EmitterShape>	_shapeEnums;
		DataArray<String>	_shapeStrings;
	};
}