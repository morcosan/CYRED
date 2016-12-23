// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_GameObject : public JsonSerializer
	{
		const Char* const NAME				= "name";
		const Char* const ENABLED			= "enabled";
		const Char* const COMPONENTS		= "components";
		const Char* const TRANSFORM			= "transform";
		const Char* const CAMERA			= "camera";
		const Char* const PARTICLE_EMITTER	= "particle_emitter";
		const Char* const MESH_RENDERING	= "mesh_rendering";
		const Char* const MORPH_RENDERING	= "morph_rendering";
		const Char* const SCRIPTER			= "scripter";


	public:
		JsonSerializer_GameObject() {}
		virtual ~JsonSerializer_GameObject() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}