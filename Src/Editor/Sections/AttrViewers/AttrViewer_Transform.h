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
		cchar* const TITLE				= "Transform";

		cchar* const ATTR_LOCAL_POS		= "L.Position";
		cchar* const ATTR_LOCAL_ROT		= "L.Rotation";
		cchar* const ATTR_LOCAL_SCALE	= "L.Scale";
		cchar* const ATTR_WORLD_POS		= "W.Position";
		cchar* const ATTR_WORLD_ROT		= "W.Rotation";
		cchar* const ATTR_WORLD_SCALE	= "W.Scale";

		cchar* const GROUP_LOCAL		= "Local";
		cchar* const GROUP_WORLD		= "World";



	public:
		AttrViewer_Transform() {}
		virtual ~AttrViewer_Transform() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		Transform* _target;
	};
}