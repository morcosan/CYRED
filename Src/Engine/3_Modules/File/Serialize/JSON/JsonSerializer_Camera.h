// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Camera : public JsonSerializer
	{
		const Char* const ENABLED			= "enabled";
		const Char* const CAMERA_TYPE		= "camera_type";
		const Char* const FOVY_ANGLE		= "fovy_angle";
		const Char* const ORTHO_HEIGHT		= "ortho_height";
		const Char* const NEAR_CLIP			= "near_clipping";
		const Char* const FAR_CLIP			= "far_clipping";


	public:
		JsonSerializer_Camera() {}
		virtual ~JsonSerializer_Camera() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}