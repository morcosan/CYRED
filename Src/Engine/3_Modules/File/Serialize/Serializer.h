// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Math/Math.h"


namespace CYRED
{
	namespace Enum_DeserFlag
	{
		enum Enum
		{
			FULL
			, UID_ONLY
		};
	}

	typedef	Enum_DeserFlag::Enum	DeserFlag;
}


namespace CYRED
{
	ABSTRACT class DLL Serializer
	{
	public:
		Serializer() {}
		virtual ~Serializer() {};


	public:
		virtual String		Serialize	( const void* object )		PURE_VIRTUAL;
		virtual void		Deserialize	( const Char* data, OUT void* object,
										  DeserFlag flag )			PURE_VIRTUAL;

		virtual String		SerializeVec2	( Vector2 value )		PURE_VIRTUAL;
		virtual String		SerializeVec3	( Vector3 value )		PURE_VIRTUAL;
		virtual String		SerializeVec4	( Vector4 value )		PURE_VIRTUAL;
		virtual Vector2		DeserializeVec2	( const Char* data )	PURE_VIRTUAL;
		virtual Vector3		DeserializeVec3	( const Char* data )	PURE_VIRTUAL;
		virtual Vector4		DeserializeVec4	( const Char* data )	PURE_VIRTUAL;
	};
}