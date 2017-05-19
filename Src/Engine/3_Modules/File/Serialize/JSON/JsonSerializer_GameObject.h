// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_GameObject : public JsonSerializer
	{
		const char* const NAME				= "name";
		const char* const ENABLED			= "enabled";
		const char* const COMPONENTS		= "components";
		const char* const TRANSFORM			= "transform";
		const char* const CAMERA			= "camera";
		const char* const LIGHT				= "light";
		const char* const PARTICLE_EMITTER	= "particle_emitter";
		const char* const MESH_RENDERING	= "mesh_rendering";
		const char* const MORPH_RENDERING	= "morph_rendering";
		const char* const SCRIPTER			= "scripter";
		const char* const CHILD_NODES		= "child_nodes";


	public:
		JsonSerializer_GameObject() {}
		virtual ~JsonSerializer_GameObject() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}