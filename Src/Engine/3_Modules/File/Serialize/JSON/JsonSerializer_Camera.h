// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Camera : public JsonSerializer
	{
		const char* const ENABLED			= "enabled";
		const char* const CAMERA_TYPE		= "camera_type";
		const char* const FOVY_ANGLE		= "fovy_angle";
		const char* const ORTHO_HEIGHT		= "ortho_height";
		const char* const NEAR_CLIP			= "near_clipping";
		const char* const FAR_CLIP			= "far_clipping";


	public:
		JsonSerializer_Camera() {}
		virtual ~JsonSerializer_Camera() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}