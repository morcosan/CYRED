// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Camera : public JsonSerializer
	{
		cchar* const ATTR_ENABLED		= "enabled";
		cchar* const ATTR_CAMERA_TYPE	= "camera_type";
		cchar* const ATTR_FOVY_ANGLE	= "fovy_angle";
		cchar* const ATTR_ORTHO_HEIGHT	= "ortho_height";
		cchar* const ATTR_NEAR_CLIP		= "near_clipping";
		cchar* const ATTR_FAR_CLIP		= "far_clipping";


	public:
		JsonSerializer_Camera() {}
		virtual ~JsonSerializer_Camera() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}