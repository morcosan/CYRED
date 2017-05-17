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
		const char* TITLE				= "Transform";

		const char* ATTR_LOCAL_POS		= "L.Position";
		const char* ATTR_LOCAL_ROT		= "L.Rotation";
		const char* ATTR_LOCAL_SCALE	= "L.Scale";
		const char* ATTR_WORLD_POS		= "W.Position";
		const char* ATTR_WORLD_ROT		= "W.Rotation";
		const char* ATTR_WORLD_SCALE	= "W.Scale";

		const char* GROUP_LOCAL			= "Local";
		const char* GROUP_WORLD			= "World";



	public:
		AttrViewer_Transform() {}
		virtual ~AttrViewer_Transform() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		Transform* _target;
	};
}