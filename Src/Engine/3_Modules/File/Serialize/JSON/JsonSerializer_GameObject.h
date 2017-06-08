// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_GameObject : public JsonSerializer
	{
		cchar* const ATTR_NAME				= "name";
		cchar* const ATTR_TAG				= "tag";
		cchar* const ATTR_LAYER				= "layer";
		cchar* const ATTR_ENABLED			= "enabled";
		cchar* const ATTR_COMPONENTS		= "components";
		cchar* const COMP_TRANSFORM			= "transform";
		cchar* const COMP_CAMERA			= "camera";
		cchar* const COMP_LIGHT				= "light";
		cchar* const COMP_PARTICLE_EMITTER	= "particle_emitter";
		cchar* const COMP_MESH_RENDERING	= "mesh_rendering";
		cchar* const COMP_MORPH_RENDERING	= "morph_rendering";
		cchar* const COMP_SCRIPTER			= "scripter";
		cchar* const COMP_RIGID_BODY		= "rigid_body";
		cchar* const COMP_TEXT_3D			= "text_3d";
		cchar* const ATTR_CHILD_NODES		= "child_nodes";


	public:
		JsonSerializer_GameObject() {}
		virtual ~JsonSerializer_GameObject() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}