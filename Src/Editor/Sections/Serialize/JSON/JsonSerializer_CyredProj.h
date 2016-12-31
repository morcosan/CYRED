// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredModule_File.h"


namespace CYRED
{
	class JsonSerializer_CyredProj : public JsonSerializer
	{
		const Char* const APP_CONFIG	= "app_config";
		const Char* const APP_NAME		= "app_name";
		const Char* const FULLSCREEN	= "fullscreen";
		const Char* const WIDTH			= "width";
		const Char* const HEIGHT		= "height";
		const Char* const POS_X			= "pos_x";
		const Char* const POS_Y			= "pos_y";
		const Char* const FPS			= "fps";
		const Char* const START_SCENE	= "start_scene";
		const Char* const BUILD_WINDOWS	= "build_dir_windows";
		const Char* const BUILD_ANDROID	= "build_dir_android";


	public:
		JsonSerializer_CyredProj() {}
		virtual ~JsonSerializer_CyredProj() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}