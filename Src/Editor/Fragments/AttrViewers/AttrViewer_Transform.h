// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	namespace COMP
	{
		class Transform;
	}
}


namespace CYRED
{
	class AttrViewer_Transform : public AttrViewer
	{
	public:
		const Char* TITLE				= "Transform";

		const Char* ATTR_LOCAL_POS		= "L.Position";
		const Char* ATTR_LOCAL_ROT		= "L.Rotation";
		const Char* ATTR_LOCAL_SCALE	= "L.Scale";
		const Char* ATTR_WORLD_POS		= "W.Position";
		const Char* ATTR_WORLD_ROT		= "W.Rotation";
		const Char* ATTR_WORLD_SCALE	= "W.Scale";

		const Char* GROUP_LOCAL			= "Local";
		const Char* GROUP_WORLD			= "World";



	public:
		AttrViewer_Transform() {}
		virtual ~AttrViewer_Transform() {}


	public:
		void OnSelect_Target	( void* target )	override;
	

	protected:
		void _OnUpdate_GUI		()	override;
		void _OnUpdate_Target	()	override;


	protected:
		COMP::Transform* _target;
	};
}