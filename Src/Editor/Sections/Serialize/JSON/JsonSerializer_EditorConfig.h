// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredModule_File.h"


namespace CYRED
{
	class JsonSerializer_EditorConfig : public JsonSerializer
	{
		cchar* const FULLSCREEN		= "fullscreen";
		cchar* const WIDTH				= "width";
		cchar* const HEIGHT			= "height";
		cchar* const POS_X				= "pos_x";
		cchar* const POS_Y				= "pos_y";
		cchar* const FPS				= "fps";
		cchar* const PROJECT_NAME		= "project_name";
		cchar* const PROJECT_PATH		= "project_path";

		cchar* const PANELS			= "panels";
		cchar* const PANELS_TYPE		= "type";
		cchar* const PANELS_SPLIT_FROM	= "split_from";
		cchar* const PANELS_SPLIT_TYPE	= "split_type";
		cchar* const PANELS_VP_INDEX	= "viewport_index";
		cchar* const HORIZONTAL		= "HORIZONTAL";
		cchar* const VERTICAL			= "VERTICAL";
		cchar* const P_ASSETS			= "ASSETS";
		cchar* const P_ATTRIBUTES		= "ATTRIBUTES";
		cchar* const P_CONSOLE			= "CONSOLE";
		cchar* const P_SCENE_HIERARCHY	= "SCENE_HIERARCHY";
		cchar* const P_SCENE_VIEWPORT	= "SCENE_VIEWPORT";
		cchar* const P_PREFAB_HIERARCHY= "PREFAB_HIERARCHY";
		cchar* const P_PREFAB_VIEWPORT	= "PREFAB_VIEWPORT";


	public:
		JsonSerializer_EditorConfig() {}
		virtual ~JsonSerializer_EditorConfig() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}