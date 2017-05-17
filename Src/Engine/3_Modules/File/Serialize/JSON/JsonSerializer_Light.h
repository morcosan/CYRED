// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Light : public JsonSerializer
	{
		const char* const ENABLED		= "enabled";
		const char* const LIGHT_TYPE	= "light_type";
		const char* const INTENSITY		= "intensity";
		const char* const COLOR			= "color";
		const char* const RANGE			= "range";
		const char* const SPOT_ANGLE	= "spot_angle";


	public:
		JsonSerializer_Light() {}
		virtual ~JsonSerializer_Light() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}