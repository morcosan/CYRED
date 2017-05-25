// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Scripter : public JsonSerializer
	{
		cchar* const ENABLED			= "enabled";
		cchar* const SCRIPTS			= "scripts";
		cchar* const SCRIPT_UID		= "uid";
		cchar* const SCRIPT_VARS		= "variables";
		cchar* const VAR_NAME			= "name";
		cchar* const VAR_TYPE			= "type";
		cchar* const VAR_VALUE			= "value";
		cchar* const VAR_TYPE_FLOAT	= "FLOAT";
		cchar* const VAR_TYPE_INT		= "INT";
		cchar* const VAR_TYPE_BOOL		= "BOOL";
		cchar* const VAR_TYPE_STRING	= "STRING";
		cchar* const VAR_TYPE_VEC2		= "VECTOR2";
		cchar* const VAR_TYPE_VEC3		= "VECTOR3";
		cchar* const VAR_TYPE_VEC4		= "VECTOR4";
		cchar* const VAR_TYPE_PREFAB	= "PREFAB";


	public:
		JsonSerializer_Scripter() {}
		virtual ~JsonSerializer_Scripter() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}