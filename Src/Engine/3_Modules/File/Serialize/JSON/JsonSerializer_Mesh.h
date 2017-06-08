// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Mesh : public JsonSerializer
	{
		cchar* const ATTR_UNIQUE_ID		= "uid";
		cchar* const ATTR_MESH_TYPE		= "mesh_type";
		cchar* const MESH_TYPE_LINE		= "LINE";
		cchar* const MESH_TYPE_POLY		= "POLYGON";
		cchar* const ATTR_LOAD_TYPE		= "load_type";
		cchar* const LOAD_TYPE_EXTERNAL	= "EXTERNAL";
		cchar* const LOAD_TYPE_SCRIPTED	= "SCRIPTED";
		cchar* const ATTR_FILE_PATH		= "file_path";
		cchar* const ATTR_CLEAR_BUFFER	= "clear_buffers_on_bind";


	public:
		JsonSerializer_Mesh() {}
		virtual ~JsonSerializer_Mesh() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}