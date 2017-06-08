// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Scene : public JsonSerializer
	{
		cchar* const ATTR_UNIQUE_ID		= "uid";
		cchar* const ATTR_GAMEOBJECTS	= "game_objects";


	public:
		JsonSerializer_Scene() {}
		virtual ~JsonSerializer_Scene() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}