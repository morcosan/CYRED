// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Light : public JsonSerializer
	{
		const Char* const ENABLED		= "enabled";
		const Char* const LIGHT_TYPE	= "light_type";
		const Char* const INTENSITY		= "intensity";
		const Char* const COLOR			= "color";
		const Char* const RANGE			= "range";
		const Char* const SPOT_ANGLE	= "spot_angle";


	public:
		JsonSerializer_Light() {}
		virtual ~JsonSerializer_Light() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}