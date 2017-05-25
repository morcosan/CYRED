// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "../SerializeSystem.h"



namespace CYRED
{
	class DLL JsonSerializeSystem : public SerializeSystem
	{
	public:
		JsonSerializeSystem();
		virtual ~JsonSerializeSystem() {}


	public:
		String		SerializeVec2	( Vector2 value )		override;
		String		SerializeVec3	( Vector3 value )		override;
		String		SerializeVec4	( Vector4 value )		override;

		Vector2		DeserializeVec2	( cchar* data )	override;
		Vector3		DeserializeVec3	( cchar* data )	override;
		Vector4		DeserializeVec4	( cchar* data )	override;
	};
}