// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Material : public JsonSerializer
	{
		cchar* const UNIQUE_ID			= "uid";
		cchar* const SHADER			= "shader";
		cchar* const WIREFRAME			= "wireframe";
		cchar* const LINE_WIDTH		= "line_width";
		cchar* const CULL_FACE			= "cull_face";
		cchar* const CULL_FACE_BACK	= "CULL_BACK";
		cchar* const CULL_FACE_FRONT	= "CULL_FRONT";
		cchar* const CULL_FACE_NONE	= "CULL_NONE";
		cchar* const PROPERTIES		= "properties";
		cchar* const PROP_NAME			= "name";
		cchar* const PROP_TYPE			= "type";
		cchar* const PROP_VALUE		= "value";
		cchar* const PROP_TYPE_FLOAT	= "FLOAT";
		cchar* const PROP_TYPE_INT		= "INT";
		cchar* const PROP_TYPE_VEC2	= "VEC2";
		cchar* const PROP_TYPE_VEC3	= "VEC3";
		cchar* const PROP_TYPE_VEC4	= "VEC4";
		cchar* const PROP_TYPE_TEX		= "TEXTURE";


	public:
		JsonSerializer_Material() {}
		virtual ~JsonSerializer_Material() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}