// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Material : public JsonSerializer
	{
		const char* const UNIQUE_ID			= "uid";
		const char* const SHADER			= "shader";
		const char* const WIREFRAME			= "wireframe";
		const char* const LINE_WIDTH		= "line_width";
		const char* const CULL_FACE			= "cull_face";
		const char* const CULL_FACE_BACK	= "CULL_BACK";
		const char* const CULL_FACE_FRONT	= "CULL_FRONT";
		const char* const CULL_FACE_NONE	= "CULL_NONE";
		const char* const PROPERTIES		= "properties";
		const char* const PROP_NAME			= "name";
		const char* const PROP_TYPE			= "type";
		const char* const PROP_VALUE		= "value";
		const char* const PROP_TYPE_FLOAT	= "FLOAT";
		const char* const PROP_TYPE_INT		= "INT";
		const char* const PROP_TYPE_VEC2	= "VEC2";
		const char* const PROP_TYPE_VEC3	= "VEC3";
		const char* const PROP_TYPE_VEC4	= "VEC4";
		const char* const PROP_TYPE_TEX		= "TEXTURE";


	public:
		JsonSerializer_Material() {}
		virtual ~JsonSerializer_Material() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}