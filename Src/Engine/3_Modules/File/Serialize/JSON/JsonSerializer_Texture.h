// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Texture : public JsonSerializer
	{
		const char* const UNIQUE_ID			= "uid";
		const char* const TEXTURE_TYPE		= "texture_type";
		const char* const TYPE_TEXTURE_2D	= "TEXTURE_2D";
		const char* const TYPE_CUBE_MAP		= "CUBE_MAP";
		const char* const LOAD_TYPE			= "load_type";
		const char* const TYPE_SCRIPTED		= "SCRIPTED";
		const char* const TYPE_EXTERNAL		= "EXTERNAL";
		const char* const HAS_MIPMAP		= "has_mipmap";
		const char* const CLEAR_BUFFER		= "clear_buffer_on_bind";
		const char* const FILE_PATH			= "file_path";
		const char* const FILE_PATH_POSX	= "file_path_pos_x";
		const char* const FILE_PATH_NEGX	= "file_path_neg_x";
		const char* const FILE_PATH_POSY	= "file_path_pos_y";
		const char* const FILE_PATH_NEGY	= "file_path_neg_y";
		const char* const FILE_PATH_POSZ	= "file_path_pos_z";
		const char* const FILE_PATH_NEGZ	= "file_path_neg_z";


	public:
		JsonSerializer_Texture() {}
		virtual ~JsonSerializer_Texture() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}