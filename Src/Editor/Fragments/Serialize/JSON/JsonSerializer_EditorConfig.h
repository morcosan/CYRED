// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredModule_File.h"


namespace CYRED
{
	class JsonSerializer_EditorConfig : public JsonSerializer
	{
		const Char* const WIDTH			= "width";
		const Char* const HEIGHT		= "height";
		const Char* const POS_X			= "pos_x";
		const Char* const POS_Y			= "pos_y";
		const Char* const FPS			= "fps";
		const Char* const PROJECT_PATH	= "project_path";
		const Char* const BUILD_PATH	= "build_path";


	public:
		JsonSerializer_EditorConfig() {}
		virtual ~JsonSerializer_EditorConfig() {};


	public:
		rapidjson::Value	ToJson	( void* object )							override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}