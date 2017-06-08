// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Light : public JsonSerializer
	{
		cchar* const ATTR_ENABLED		= "enabled";
		cchar* const ATTR_LIGHT_TYPE	= "light_type";
		cchar* const ATTR_INTENSITY		= "intensity";
		cchar* const ATTR_COLOR			= "color";
		cchar* const ATTR_RANGE			= "range";
		cchar* const ATTR_SPOT_ANGLE	= "spot_angle";


	public:
		JsonSerializer_Light() {}
		virtual ~JsonSerializer_Light() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}