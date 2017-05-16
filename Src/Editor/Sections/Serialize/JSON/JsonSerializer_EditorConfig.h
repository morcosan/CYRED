// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredModule_File.h"


namespace CYRED
{
	class JsonSerializer_EditorConfig : public JsonSerializer
	{
		const Char* const FULLSCREEN		= "fullscreen";
		const Char* const WIDTH				= "width";
		const Char* const HEIGHT			= "height";
		const Char* const POS_X				= "pos_x";
		const Char* const POS_Y				= "pos_y";
		const Char* const FPS				= "fps";
		const Char* const PROJECT_NAME		= "project_name";
		const Char* const PROJECT_PATH		= "project_path";

		const Char* const PANELS			= "panels";
		const Char* const PANELS_TYPE		= "type";
		const Char* const PANELS_SPLIT_FROM	= "split_from";
		const Char* const PANELS_SPLIT_TYPE	= "split_type";
		const Char* const PANELS_VP_INDEX	= "viewport_index";
		const Char* const HORIZONTAL		= "HORIZONTAL";
		const Char* const VERTICAL			= "VERTICAL";
		const Char* const P_ASSETS			= "ASSETS";
		const Char* const P_ATTRIBUTES		= "ATTRIBUTES";
		const Char* const P_CONSOLE			= "CONSOLE";
		const Char* const P_SCENE_HIERARCHY	= "SCENE_HIERARCHY";
		const Char* const P_SCENE_VIEWPORT	= "SCENE_VIEWPORT";
		const Char* const P_PREFAB_HIERARCHY= "PREFAB_HIERARCHY";
		const Char* const P_PREFAB_VIEWPORT	= "PREFAB_VIEWPORT";


	public:
		JsonSerializer_EditorConfig() {}
		virtual ~JsonSerializer_EditorConfig() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}