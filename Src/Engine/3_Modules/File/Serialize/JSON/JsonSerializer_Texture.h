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
		const Char* const HAS_MIPMAP		= "has_mipmap";
		const Char* const CLEAR_BUFFER		= "clear_buffer_on_bind";
		const Char* const FILE_PATH			= "file_path";
		const Char* const FILE_PATH_POSX	= "file_path_pos_x";
		const Char* const FILE_PATH_NEGX	= "file_path_neg_x";
		const Char* const FILE_PATH_POSY	= "file_path_pos_y";
		const Char* const FILE_PATH_NEGY	= "file_path_neg_y";
		const Char* const FILE_PATH_POSZ	= "file_path_pos_z";
		const Char* const FILE_PATH_NEGZ	= "file_path_neg_z";


	public:
		JsonSerializer_Texture() {}
		virtual ~JsonSerializer_Texture() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}