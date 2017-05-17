// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_MorphRendering : public JsonSerializer
	{
		const char* const ENABLED		= "enabled";
		const char* const MORPH			= "morph";
		const char* const MATERIAL		= "material";
		const char* const DUR_STATE		= "duration_state";
		const char* const DUR_CHANGE	= "duration_state_change";
		const char* const IS_PLAYING	= "is_playing";


	public:
		JsonSerializer_MorphRendering() {}
		virtual ~JsonSerializer_MorphRendering() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}