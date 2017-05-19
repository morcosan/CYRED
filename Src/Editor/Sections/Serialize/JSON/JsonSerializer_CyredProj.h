// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredModule_File.h"


namespace CYRED
{
	class JsonSerializer_CyredProj : public JsonSerializer
	{
		const char* const APP_CONFIG	= "app_config";
		const char* const APP_NAME		= "app_name";
		const char* const FULLSCREEN	= "fullscreen";
		const char* const WIDTH			= "width";
		const char* const HEIGHT		= "height";
		const char* const POS_X			= "pos_x";
		const char* const POS_Y			= "pos_y";
		const char* const FPS			= "fps";
		const char* const START_SCENE	= "start_scene";
		const char* const BUILD_WINDOWS	= "build_dir_windows";
		const char* const BUILD_ANDROID	= "build_dir_android";


	public:
		JsonSerializer_CyredProj() {}
		virtual ~JsonSerializer_CyredProj() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}