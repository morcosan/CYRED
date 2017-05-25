// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_GameObject : public JsonSerializer
	{
		cchar* const NAME				= "name";
		cchar* const ENABLED			= "enabled";
		cchar* const COMPONENTS		= "components";
		cchar* const TRANSFORM			= "transform";
		cchar* const CAMERA			= "camera";
		cchar* const LIGHT				= "light";
		cchar* const PARTICLE_EMITTER	= "particle_emitter";
		cchar* const MESH_RENDERING	= "mesh_rendering";
		cchar* const MORPH_RENDERING	= "morph_rendering";
		cchar* const SCRIPTER			= "scripter";
		cchar* const CHILD_NODES		= "child_nodes";


	public:
		JsonSerializer_GameObject() {}
		virtual ~JsonSerializer_GameObject() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}