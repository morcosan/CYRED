// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Shader : public JsonSerializer
	{
		cchar* const UNIQUE_ID				= "uid";
		cchar* const VERTEX_FILE_PATH		= "vertex_file_path";
		cchar* const GEOMETRY_FILE_PATH	= "geometry_file_path";
		cchar* const FRAGMENT_FILE_PATH	= "fragment_file_path";


	public:
		JsonSerializer_Shader() {}
		virtual ~JsonSerializer_Shader() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}