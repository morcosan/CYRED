// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class JsonSerializer_AppConfig : public JsonSerializer
	{
		cchar* const ATTR_APP_NAME		= "app_name";
		cchar* const ATTR_FULLSCREEN	= "fullscreen";
		cchar* const ATTR_WIDTH			= "width";
		cchar* const ATTR_HEIGHT		= "height";
		cchar* const ATTR_POS_X			= "pos_x";
		cchar* const ATTR_POS_Y			= "pos_y";
		cchar* const ATTR_FPS			= "fps";
		cchar* const ATTR_START_SCENE	= "start_scene";
		cchar* const ATTR_ASSETS		= "assets";
		cchar* const ASSETS_MATERIALS	= "materials";
		cchar* const ASSETS_TEXTURES	= "textures";
		cchar* const ASSETS_SHADERS		= "shaders";
		cchar* const ASSETS_MESHES		= "meshes";
		cchar* const ASSETS_MORPHS		= "morphs";
		cchar* const ASSETS_SCENES		= "scenes";
		cchar* const ASSETS_SCRIPTS		= "scripts";
		cchar* const ASSET_NAME			= "name";
		cchar* const ASSET_PATH			= "path";


	public:
		JsonSerializer_AppConfig() {}
		virtual ~JsonSerializer_AppConfig() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}