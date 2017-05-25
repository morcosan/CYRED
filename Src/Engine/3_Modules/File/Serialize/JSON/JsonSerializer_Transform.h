// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Transform : public JsonSerializer
	{
		cchar* const ENABLED			= "enabled";
		cchar* const POSITION_WORLD	= "position_world";
		cchar* const ROTATIO_WORLD		= "rotation_world";
		cchar* const SCALE_WORLD		= "scale_world";


	public:
		JsonSerializer_Transform() {}
		virtual ~JsonSerializer_Transform() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}