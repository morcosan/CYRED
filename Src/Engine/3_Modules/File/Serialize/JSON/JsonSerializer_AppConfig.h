// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class JsonSerializer_AppConfig : public JsonSerializer
	{
		cchar* const APP_NAME			= "app_name";
		cchar* const FULLSCREEN		= "fullscreen";
		cchar* const WIDTH				= "width";
		cchar* const HEIGHT			= "height";
		cchar* const POS_X				= "pos_x";
		cchar* const POS_Y				= "pos_y";
		cchar* const FPS				= "fps";
		cchar* const START_SCENE		= "start_scene";
		cchar* const ASSETS			= "assets";
		cchar* const ASSETS_MATERIALS	= "materials";
		cchar* const ASSETS_TEXTURES	= "textures";
		cchar* const ASSETS_SHADERS	= "shaders";
		cchar* const ASSETS_MESHES		= "meshes";
		cchar* const ASSETS_MORPHS		= "morphs";
		cchar* const ASSETS_SCENES		= "scenes";
		cchar* const ASSETS_SCRIPTS	= "scripts";
		cchar* const ASSET_NAME		= "name";
		cchar* const ASSET_PATH		= "path";


	public:
		JsonSerializer_AppConfig() {}
		virtual ~JsonSerializer_AppConfig() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}