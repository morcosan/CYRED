// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Mesh : public JsonSerializer
	{
		const Char* const UNIQUE_ID			= "uid";
		const Char* const MESH_TYPE			= "mesh_type";
		const Char* const MESH_TYPE_LINE	= "LINE";
		const Char* const MESH_TYPE_POLY	= "POLYGON";
		const Char* const LOAD_TYPE			= "load_type";
		const Char* const LOAD_TYPE_EXT		= "EXTERNAL";
		const Char* const LOAD_TYPE_CUBE	= "GEN_CUBE";
		const Char* const LOAD_TYPE_QUAD	= "GEN_QUAD";
		const Char* const EXT_FILE_PATH		= "file_path";
		const Char* const CLEAR_BUFFER		= "clear_buffers_on_bind";


	public:
		JsonSerializer_Mesh() {}
		virtual ~JsonSerializer_Mesh() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}