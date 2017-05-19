// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class JsonSerializer_AppConfig : public JsonSerializer
	{
		const char* const APP_NAME			= "app_name";
		const char* const FULLSCREEN		= "fullscreen";
		const char* const WIDTH				= "width";
		const char* const HEIGHT			= "height";
		const char* const POS_X				= "pos_x";
		const char* const POS_Y				= "pos_y";
		const char* const FPS				= "fps";
		const char* const START_SCENE		= "start_scene";
		const char* const ASSETS			= "assets";
		const char* const ASSETS_MATERIALS	= "materials";
		const char* const ASSETS_TEXTURES	= "textures";
		const char* const ASSETS_SHADERS	= "shaders";
		const char* const ASSETS_MESHES		= "meshes";
		const char* const ASSETS_MORPHS		= "morphs";
		const char* const ASSETS_SCENES		= "scenes";
		const char* const ASSETS_SCRIPTS	= "scripts";
		const char* const ASSET_NAME		= "name";
		const char* const ASSET_PATH		= "path";


	public:
		JsonSerializer_AppConfig() {}
		virtual ~JsonSerializer_AppConfig() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}