// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredModule_File.h"


namespace CYRED
{
	class JsonSerializer_CyredProj : public JsonSerializer
	{
		cchar* const APP_CONFIG	= "app_config";
		cchar* const APP_NAME		= "app_name";
		cchar* const FULLSCREEN	= "fullscreen";
		cchar* const WIDTH			= "width";
		cchar* const HEIGHT		= "height";
		cchar* const POS_X			= "pos_x";
		cchar* const POS_Y			= "pos_y";
		cchar* const FPS			= "fps";
		cchar* const START_SCENE	= "start_scene";
		cchar* const BUILD_WINDOWS	= "build_dir_windows";
		cchar* const BUILD_ANDROID	= "build_dir_android";


	public:
		JsonSerializer_CyredProj() {}
		virtual ~JsonSerializer_CyredProj() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}