// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class JsonSerializer_AssetDB : public JsonSerializer
	{
		const Char* const ASSETS		= "assets";
		const Char* const ASSET_NAME	= "name";
		const Char* const ASSET_TYPE	= "type";
		const Char* const ASSET_DIR		= "dir";
		const Char* const TYPE_MATERIAL	= "MATERIAL";
		const Char* const TYPE_TEXTURE	= "TEXTURE";
		const Char* const TYPE_SHADER	= "SHADER";


	public:
		JsonSerializer_AssetDB() {}
		virtual ~JsonSerializer_AssetDB() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}