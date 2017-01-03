// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Scripter : public JsonSerializer
	{
		const Char* const ENABLED			= "enabled";
		const Char* const SCRIPTS			= "scripts";
		const Char* const SCRIPT_UID		= "uid";
		const Char* const SCRIPT_VARS		= "variables";
		const Char* const VAR_NAME			= "name";
		const Char* const VAR_TYPE			= "type";
		const Char* const VAR_VALUE			= "value";
		const Char* const VAR_TYPE_FLOAT	= "FLOAT";
		const Char* const VAR_TYPE_INT		= "INT";
		const Char* const VAR_TYPE_BOOL		= "BOOL";
		const Char* const VAR_TYPE_STRING	= "STRING";
		const Char* const VAR_TYPE_VEC2		= "VECTOR2";
		const Char* const VAR_TYPE_VEC3		= "VECTOR3";
		const Char* const VAR_TYPE_VEC4		= "VECTOR4";
		const Char* const VAR_TYPE_TEX		= "TEXTURE";


	public:
		JsonSerializer_Scripter() {}
		virtual ~JsonSerializer_Scripter() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}