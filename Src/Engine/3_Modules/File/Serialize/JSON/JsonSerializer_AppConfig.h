// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class JsonSerializer_AppConfig : public JsonSerializer
	{
		const Char* const APP_NAME			= "app_name";
		const Char* const FULLSCREEN		= "fullscreen";
		const Char* const WIDTH				= "width";
		const Char* const HEIGHT			= "height";
		const Char* const POS_X				= "pos_x";
		const Char* const POS_Y				= "pos_y";
		const Char* const FPS				= "fps";
		const Char* const START_SCENE		= "start_scene";
		const Char* const ASSETS			= "assets";
		const Char* const ASSETS_MATERIALS	= "materials";
		const Char* const ASSETS_TEXTURES	= "textures";
		const Char* const ASSETS_SHADERS	= "shaders";
		const Char* const ASSETS_MESHES		= "meshes";
		const Char* const ASSETS_MORPHS		= "morphs";
		const Char* const ASSETS_SCENES		= "scenes";
		const Char* const ASSET_NAME		= "name";
		const Char* const ASSET_PATH		= "path";


	public:
		JsonSerializer_AppConfig() {}
		virtual ~JsonSerializer_AppConfig() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}