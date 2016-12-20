// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class JsonSerializer_SceneDB : public JsonSerializer
	{
		const Char* const SCENE			= "scene";
		const Char* const ASSETS		= "assets";
		const Char* const ASSET_TYPE	= "type";
		const Char* const ASSET_PATH	= "path";
		const Char* const TYPE_MATERIAL	= "MATERIAL";
		const Char* const TYPE_TEXTURE	= "TEXTURE";
		const Char* const TYPE_SHADER	= "SHADER";


	public:
		JsonSerializer_SceneDB() {}
		virtual ~JsonSerializer_SceneDB() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}