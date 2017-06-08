// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_MorphRendering : public JsonSerializer
	{
		cchar* const ATTR_ENABLED		= "enabled";
		cchar* const ATTR_MORPH			= "morph";
		cchar* const ATTR_MATERIAL		= "material";
		cchar* const ATTR_DUR_STATE		= "duration_state";
		cchar* const ATTR_DUR_CHANGE	= "duration_state_change";
		cchar* const ATTR_IS_PLAYING	= "is_playing";


	public:
		JsonSerializer_MorphRendering() {}
		virtual ~JsonSerializer_MorphRendering() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}