// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Morph : public JsonSerializer
	{
		const Char* const UNIQUE_ID			= "uid";
		const Char* const FILE_PATHS		= "file_paths";


	public:
		JsonSerializer_Morph() {}
		virtual ~JsonSerializer_Morph() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}