// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Scripter : public JsonSerializer
	{
		const char* const ENABLED			= "enabled";
		const char* const SCRIPTS			= "scripts";
		const char* const SCRIPT_UID		= "uid";
		const char* const SCRIPT_VARS		= "variables";
		const char* const VAR_NAME			= "name";
		const char* const VAR_TYPE			= "type";
		const char* const VAR_VALUE			= "value";
		const char* const VAR_TYPE_FLOAT	= "FLOAT";
		const char* const VAR_TYPE_INT		= "INT";
		const char* const VAR_TYPE_BOOL		= "BOOL";
		const char* const VAR_TYPE_STRING	= "STRING";
		const char* const VAR_TYPE_VEC2		= "VECTOR2";
		const char* const VAR_TYPE_VEC3		= "VECTOR3";
		const char* const VAR_TYPE_VEC4		= "VECTOR4";
		const char* const VAR_TYPE_PREFAB	= "PREFAB";


	public:
		JsonSerializer_Scripter() {}
		virtual ~JsonSerializer_Scripter() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}