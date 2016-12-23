// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Scripter : public JsonSerializer
	{
		const Char* const ENABLED	= "enabled";
		const Char* const SCRIPT	= "script";


	public:
		JsonSerializer_Scripter() {}
		virtual ~JsonSerializer_Scripter() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}