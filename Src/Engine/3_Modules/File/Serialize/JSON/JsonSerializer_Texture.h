// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Texture : public JsonSerializer
	{
		const Char* const UNIQUE_ID			= "uid";
		const Char* const TYPE				= "type";
		const Char* const TYPE_TEXTURE_2D	= "TEXTURE_2D";
		const Char* const TYPE_CUBE_MAP		= "CUBE_MAP";
		const Char* const HAS_MIPMAP		= "hasMipmap";
		const Char* const CLEAR_BUFFER		= "clearBufferOnBind";
		const Char* const FILE_PATH			= "filePath";
		const Char* const FILE_PATH_POSX	= "filePathPosX";
		const Char* const FILE_PATH_NEGX	= "filePathNegX";
		const Char* const FILE_PATH_POSY	= "filePathPosY";
		const Char* const FILE_PATH_NEGY	= "filePathNegY";
		const Char* const FILE_PATH_POSZ	= "filePathPosZ";
		const Char* const FILE_PATH_NEGZ	= "filePathNegZ";


	public:
		JsonSerializer_Texture() {}
		virtual ~JsonSerializer_Texture() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}