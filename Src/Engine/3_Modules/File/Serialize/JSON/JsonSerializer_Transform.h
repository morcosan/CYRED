// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Transform : public JsonSerializer
	{
		const Char* const ENABLED			= "Enabled";
		const Char* const POSITION_WORLD	= "Position_World";
		const Char* const ROTATIO_WORLD		= "Rotation_World";
		const Char* const SCALE_WORLD		= "Scale_World";


	public:
		JsonSerializer_Transform() {}
		virtual ~JsonSerializer_Transform() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}