// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Mesh : public JsonSerializer
	{
		const char* const UNIQUE_ID				= "uid";
		const char* const MESH_TYPE				= "mesh_type";
		const char* const MESH_TYPE_LINE		= "LINE";
		const char* const MESH_TYPE_POLY		= "POLYGON";
		const char* const LOAD_TYPE				= "load_type";
		const char* const LOAD_TYPE_EXTERNAL	= "EXTERNAL";
		const char* const LOAD_TYPE_SCRIPTED	= "SCRIPTED";
		const char* const FILE_PATH				= "file_path";
		const char* const CLEAR_BUFFER			= "clear_buffers_on_bind";


	public:
		JsonSerializer_Mesh() {}
		virtual ~JsonSerializer_Mesh() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}