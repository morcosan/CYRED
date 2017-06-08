// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_MeshRendering : public JsonSerializer
	{
		cchar* const ATTR_ENABLED	= "enabled";
		cchar* const ATTR_MESH		= "mesh";
		cchar* const ATTR_MATERIAL	= "material";


	public:
		JsonSerializer_MeshRendering() {}
		virtual ~JsonSerializer_MeshRendering() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}