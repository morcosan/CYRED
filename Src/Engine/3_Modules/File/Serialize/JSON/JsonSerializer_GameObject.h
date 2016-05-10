// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_GameObject : public JsonSerializer
	{
		const Char* const NAME				= "Name";
		const Char* const ENABLED			= "Enabled";
		const Char* const COMPONENTS		= "Components";
		const Char* const TRANSFORM			= "Transform";
		const Char* const CAMERA			= "Camera";
		const Char* const PARTICLE_EMITTER	= "ParticleEmitter";
		const Char* const MESH_RENDERING	= "MeshRendering";
		const Char* const MORPH_RENDERING	= "MorphRendering";


	public:
		JsonSerializer_GameObject() {}
		virtual ~JsonSerializer_GameObject() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}