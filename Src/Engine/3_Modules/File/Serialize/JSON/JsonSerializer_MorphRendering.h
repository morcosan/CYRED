// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_MorphRendering : public JsonSerializer
	{
		cchar* const ENABLED		= "enabled";
		cchar* const MORPH			= "morph";
		cchar* const MATERIAL		= "material";
		cchar* const DUR_STATE		= "duration_state";
		cchar* const DUR_CHANGE	= "duration_state_change";
		cchar* const IS_PLAYING	= "is_playing";


	public:
		JsonSerializer_MorphRendering() {}
		virtual ~JsonSerializer_MorphRendering() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}