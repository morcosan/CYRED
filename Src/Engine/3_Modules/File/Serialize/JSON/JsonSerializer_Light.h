// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Light : public JsonSerializer
	{
		cchar* const ENABLED		= "enabled";
		cchar* const LIGHT_TYPE	= "light_type";
		cchar* const INTENSITY		= "intensity";
		cchar* const COLOR			= "color";
		cchar* const RANGE			= "range";
		cchar* const SPOT_ANGLE	= "spot_angle";


	public:
		JsonSerializer_Light() {}
		virtual ~JsonSerializer_Light() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}