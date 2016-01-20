// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredModule_File.h"


namespace CYRED
{
	class JsonSerializer_GameConfig : public JsonSerializer
	{
		const Char* const APP_NAME		= "app_name";
		const Char* const FULLSCREEN	= "fullscreen";
		const Char* const WIDTH			= "width";
		const Char* const HEIGHT		= "height";
		const Char* const POS_X			= "pos_x";
		const Char* const POS_Y			= "pos_y";
		const Char* const FPS			= "fps";
		const Char* const SCENES		= "scenes";
		const Char* const SCENE_NAME	= "scene_name";
		const Char* const SCENE_DIR		= "scene_dir";
		const Char* const ASSETDB_DIR	= "assetdb_dir";


	public:
		JsonSerializer_GameConfig() {}
		virtual ~JsonSerializer_GameConfig() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}