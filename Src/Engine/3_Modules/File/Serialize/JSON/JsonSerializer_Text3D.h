// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "JsonSerializer.h"


namespace CYRED
{
	class DLL JsonSerializer_Text3D : public JsonSerializer
	{
		cchar* const ATTR_ENABLED		= "enabled";
		cchar* const ATTR_TEXT			= "Text";
		cchar* const ATTR_TEXT_COLOR	= "Color";
		cchar* const ATTR_V_ALIGN		= "V. Align";
		cchar* const ATTR_H_ALIGN		= "H. Align";
		cchar* const ATTR_FONT			= "Font";
		cchar* const ATTR_FONT_SIZE		= "Size";
		cchar* const ATTR_MATERIAL		= "Material";
		cchar* const V_ALIN_TOP			= "TOP";
		cchar* const V_ALIN_MIDDLE		= "MIDDLE";
		cchar* const V_ALIN_BOTTOM		= "BOTTOM";
		cchar* const H_ALIN_LEFT		= "LEFT";
		cchar* const H_ALIN_MIDDLE		= "MIDDLE";
		cchar* const H_ALIN_RIGHT		= "RIGHT";


	public:
		JsonSerializer_Text3D() {}
		virtual ~JsonSerializer_Text3D() {};


	public:
		rapidjson::Value	ToJson	( const void* object )						override;
		void				FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )							override;
	};
}