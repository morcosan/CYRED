// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Texture : public JsonSerializer
	{
		cchar* const ATTR_UNIQUE_ID			= "uid";
		cchar* const ATTR_TEXTURE_TYPE		= "texture_type";
		cchar* const TYPE_TEXTURE_2D		= "TEXTURE_2D";
		cchar* const TYPE_CUBE_MAP			= "CUBE_MAP";
		cchar* const ATTR_LOAD_TYPE			= "load_type";
		cchar* const TYPE_SCRIPTED			= "SCRIPTED";
		cchar* const TYPE_EXTERNAL			= "EXTERNAL";
		cchar* const ATTR_HAS_MIPMAP		= "has_mipmap";
		cchar* const ATTR_CLEAR_BUFFER		= "clear_buffer_on_bind";
		cchar* const ATTR_FILE_PATH			= "file_path";
		cchar* const ATTR_FILE_PATH_POSX	= "file_path_pos_x";
		cchar* const ATTR_FILE_PATH_NEGX	= "file_path_neg_x";
		cchar* const ATTR_FILE_PATH_POSY	= "file_path_pos_y";
		cchar* const ATTR_FILE_PATH_NEGY	= "file_path_neg_y";
		cchar* const ATTR_FILE_PATH_POSZ	= "file_path_pos_z";
		cchar* const ATTR_FILE_PATH_NEGZ	= "file_path_neg_z";


	public:
		JsonSerializer_Texture() {}
		virtual ~JsonSerializer_Texture() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}