// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Prefab : public JsonSerializer
	{
		cchar* const UNIQUE_ID	= "uid";
		cchar* const ROOT		= "root";


	public:
		JsonSerializer_Prefab() {}
		virtual ~JsonSerializer_Prefab() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}