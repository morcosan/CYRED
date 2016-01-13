// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Shader : public JsonSerializer
	{
		const Char* const UNIQUE_ID				= "uid";
		const Char* const RENDERER				= "renderer";
		const Char* const RENDERER_FORWARD		= "FORWARD";
		const Char* const VERTEX_FILE_PATH		= "vertexFilePath";
		const Char* const GEOMETRY_FILE_PATH	= "geometryFilePath";
		const Char* const FRAGMENT_FILE_PATH	= "fragmentFilePath";


	public:
		JsonSerializer_Shader() {}
		virtual ~JsonSerializer_Shader() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}