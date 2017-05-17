// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Scene : public JsonSerializer
	{
		const char* const UNIQUE_ID		= "uid";
		const char* const GAMEOBJECTS	= "game_objects";


	public:
		JsonSerializer_Scene() {}
		virtual ~JsonSerializer_Scene() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}