// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Material : public JsonSerializer
	{
		const Char* const UNIQUE_ID			= "uid";
		const Char* const SHADER			= "shader";
		const Char* const WIREFRAME			= "wireframe";
		const Char* const LINE_WIDTH		= "lineWidth";
		const Char* const CULL_FACE			= "cullFace";
		const Char* const CULL_FACE_BACK	= "CULL_BACK";
		const Char* const CULL_FACE_FRONT	= "CULL_FRONT";
		const Char* const CULL_FACE_NONE	= "CULL_NONE";
		const Char* const PROPERTIES		= "properties";
		const Char* const PROP_NAME			= "name";
		const Char* const PROP_TYPE			= "type";
		const Char* const PROP_VALUE		= "value";
		const Char* const PROP_TYPE_FLOAT	= "FLOAT";
		const Char* const PROP_TYPE_INT		= "INT";
		const Char* const PROP_TYPE_VEC2	= "VEC2";
		const Char* const PROP_TYPE_VEC3	= "VEC3";
		const Char* const PROP_TYPE_VEC4	= "VEC4";
		const Char* const PROP_TYPE_TEX		= "TEXTURE";


	public:
		JsonSerializer_Material() {}
		virtual ~JsonSerializer_Material() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}