// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Font : public JsonSerializer
	{
		cchar* const ATTR_UNIQUE_ID	= "uid";
		cchar* const ATTR_FILE_PATH	= "file_path";
		cchar* const ATTR_MAX_SIZE	= "max_size";


	public:
		JsonSerializer_Font() {}
		virtual ~JsonSerializer_Font() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}