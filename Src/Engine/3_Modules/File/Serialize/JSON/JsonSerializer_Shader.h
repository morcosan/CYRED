// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Shader : public JsonSerializer
	{
		const char* const UNIQUE_ID				= "uid";
		const char* const RENDERER				= "renderer";
		const char* const RENDERER_FORWARD		= "FORWARD";
		const char* const VERTEX_FILE_PATH		= "vertex_file_path";
		const char* const GEOMETRY_FILE_PATH	= "geometry_file_path";
		const char* const FRAGMENT_FILE_PATH	= "fragment_file_path";


	public:
		JsonSerializer_Shader() {}
		virtual ~JsonSerializer_Shader() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}