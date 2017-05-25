// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		cchar* TITLE				= "Transform";

		cchar* ATTR_LOCAL_POS		= "L.Position";
		cchar* ATTR_LOCAL_ROT		= "L.Rotation";
		cchar* ATTR_LOCAL_SCALE	= "L.Scale";
		cchar* ATTR_WORLD_POS		= "W.Position";
		cchar* ATTR_WORLD_ROT		= "W.Rotation";
		cchar* ATTR_WORLD_SCALE	= "W.Scale";

		cchar* GROUP_LOCAL			= "Local";
		cchar* GROUP_WORLD			= "World";



	public:
		AttrViewer_Transform() {}
		virtual ~AttrViewer_Transform() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	private:
		Transform* _target;
	};
}