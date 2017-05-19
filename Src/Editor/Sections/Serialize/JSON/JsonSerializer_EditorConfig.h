// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredModule_File.h"


namespace CYRED
{
	class JsonSerializer_EditorConfig : public JsonSerializer
	{
		const char* const FULLSCREEN		= "fullscreen";
		const char* const WIDTH				= "width";
		const char* const HEIGHT			= "height";
		const char* const POS_X				= "pos_x";
		const char* const POS_Y				= "pos_y";
		const char* const FPS				= "fps";
		const char* const PROJECT_NAME		= "project_name";
		const char* const PROJECT_PATH		= "project_path";

		const char* const PANELS			= "panels";
		const char* const PANELS_TYPE		= "type";
		const char* const PANELS_SPLIT_FROM	= "split_from";
		const char* const PANELS_SPLIT_TYPE	= "split_type";
		const char* const PANELS_VP_INDEX	= "viewport_index";
		const char* const HORIZONTAL		= "HORIZONTAL";
		const char* const VERTICAL			= "VERTICAL";
		const char* const P_ASSETS			= "ASSETS";
		const char* const P_ATTRIBUTES		= "ATTRIBUTES";
		const char* const P_CONSOLE			= "CONSOLE";
		const char* const P_SCENE_HIERARCHY	= "SCENE_HIERARCHY";
		const char* const P_SCENE_VIEWPORT	= "SCENE_VIEWPORT";
		const char* const P_PREFAB_HIERARCHY= "PREFAB_HIERARCHY";
		const char* const P_PREFAB_VIEWPORT	= "PREFAB_VIEWPORT";


	public:
		JsonSerializer_EditorConfig() {}
		virtual ~JsonSerializer_EditorConfig() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}