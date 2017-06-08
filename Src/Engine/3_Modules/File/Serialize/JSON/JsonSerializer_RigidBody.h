// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_RigidBody : public JsonSerializer
	{
		cchar* const ATTR_ENABLED		= "enabled";
		cchar* const ATTR_IS_TRIGGER	= "is_trigger";
		cchar* const ATTR_SHAPE_TYPE	= "shape_type";
		cchar* const TYPE_BOX			= "BOX";
		cchar* const TYPE_SPHERE		= "SPHERE";
		cchar* const ATTR_SHAPE_SIZE	= "shape_size";
		cchar* const ATTR_MASS			= "mass";


	public:
		JsonSerializer_RigidBody() {}
		virtual ~JsonSerializer_RigidBody() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}